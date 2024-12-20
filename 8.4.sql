CREATE TABLE products (
    id SERIAL PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    quantity INTEGER NOT NULL DEFAULT 0
);

CREATE TABLE operations_log (
    id SERIAL PRIMARY KEY,
    product_id INTEGER REFERENCES products(id) ON DELETE CASCADE,
    operation VARCHAR(4) NOT NULL CHECK (operation IN ('ADD', 'REMOVE')),
    quantity INTEGER NOT NULL,
    operation_time TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
);


CREATE OR REPLACE PROCEDURE update_stock(p_product_id INT, p_operation VARCHAR(4), p_quantity INT)
LANGUAGE plpgsql
AS $$
DECLARE
    v_current_quantity INTEGER;
BEGIN
    IF p_operation NOT IN ('ADD', 'REMOVE') THEN
        RAISE EXCEPTION 'Invalid operation type: %', p_operation;
    END IF;
    IF p_quantity <= 0 THEN
        RAISE EXCEPTION 'Quantity must be greater than 0';
    END IF;

    SELECT quantity INTO v_current_quantity FROM products WHERE id = p_product_id;
    IF NOT FOUND THEN
        RAISE EXCEPTION 'Product not found';
    END IF;

    IF p_operation = 'ADD' THEN
        UPDATE products SET quantity = v_current_quantity + p_quantity WHERE id = p_product_id;
    ELSIF p_operation = 'REMOVE' THEN
        IF v_current_quantity - p_quantity < 0 THEN
            RAISE EXCEPTION 'Not enough stock';
        END IF;
        UPDATE products SET quantity = v_current_quantity - p_quantity WHERE id = p_product_id;
    END IF;

    INSERT INTO operations_log (product_id, operation, quantity)
    VALUES (p_product_id, p_operation, p_quantity);

EXCEPTION WHEN OTHERS THEN
    GET STACKED DIAGNOSTICS err_context = PG_EXCEPTION_CONTEXT;
    RAISE EXCEPTION '%  %', SQLERRM, err_context;
END;
$$;

CALL update_stock(1, 'ADD', 10);

CALL update_stock(1, 'REMOVE', 5);

SELECT * FROM products;
SELECT * FROM operations_log;
