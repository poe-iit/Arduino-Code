CREATE TABLE [IF NOT EXISTS] sensors(
    column_number INT PRIMARY KEY NOT NULL AUTOINCREMENT,
    sensor_id TEXT NOT NULL,
    value TEXT,
)

INSERT INTO myapp_sensors_table(sensor_id) VALUES ('flame1'),('flame2'),('flame3'),('flame4'),('flame5'),('flame6'),('flame7'),('flame8'),
    ('smoke1'),('smoke2'),('smoke3'),('smoke4'),('smoke5'),('smoke6'),('smoke7'),('smoke8'),('motion'),('humidity'),('temperature1'),
	('temperature2'),('water');