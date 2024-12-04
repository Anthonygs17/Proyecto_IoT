CREATE TABLE feeding_data (
	id SERIAL,
	datetime TIMESTAMP DEFAULT CURRENT_TIMESTAMP(0) NOT NULL,
	comida DECIMAL(10,2) NOT NULL,
	agua DECIMAL(10,2) NOT NULL,
	PRIMARY KEY (id)
);

INSERT INTO feeding_data (comida, agua) VALUES (120, 200);
INSERT INTO feeding_data (comida, agua) VALUES (220, 350);
INSERT INTO feeding_data (comida, agua) VALUES (70, 120);

-- select comida, agua from feeding_data order by datetime desc, id desc limit 1;
