CREATE TABLE students (
    id INTEGER,
    student_name TEXT,
    PRIMARY KEY(id)
);

CREATE TABLE houses (
    id INTEGER,
    house_name TEXT,
    head TEXT,
    PRIMARY KEY(id)
);

CREATE TABLE assignments (
    id INTEGER,
    student_id INTEGER,
    house_id INTEGER,
    PRIMARY KEY(id),
    FOREIGN KEY(student_id) REFERENCES students(id),
    FOREIGN KEY(house_id) REFERENCES houses(id)
);