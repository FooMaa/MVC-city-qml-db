CREATE TABLE Classes (
    number_classes INT PRIMARY KEY,
    class_name VARCHAR(100) NOT NULL
    );

CREATE TABLE Object (
    id_object SERIAL PRIMARY KEY,
    number_classes INT REFERENCES Classes(number_classes),
    property VARCHAR(50)
    );
  
CREATE TABLE Hierarchy (
    id_child INT REFERENCES Object(id_object),
    id_parent INT REFERENCES Object(id_object)
    );
 
INSERT INTO Classes VALUES(0, 'ROOT');
INSERT INTO Classes VALUES(1, 'CITY');
INSERT INTO Classes VALUES(2, 'STREET');
INSERT INTO Classes VALUES(3, 'HOME');
INSERT INTO Classes VALUES(4, 'PEOPLE');

INSERT INTO object VALUES(0, 0, NULL);
INSERT INTO object (number_classes, property) VALUES (1, NULL);
INSERT INTO object (number_classes, property) VALUES (2, NULL);
INSERT INTO object (number_classes, property) VALUES (2, NULL);
INSERT INTO object (number_classes, property) VALUES (2, NULL);
INSERT INTO object (number_classes, property) VALUES (3, NULL);
INSERT INTO object (number_classes, property) VALUES (3, NULL);
INSERT INTO object (number_classes, property) VALUES (3, NULL);
INSERT INTO object (number_classes, property) VALUES (4, NULL);
INSERT INTO object (number_classes, property) VALUES (4, NULL);

INSERT INTO Hierarchy VALUES(1, 0);
INSERT INTO Hierarchy VALUES(2, 1);
INSERT INTO Hierarchy VALUES(3, 1);
INSERT INTO Hierarchy VALUES(4, 1);
INSERT INTO Hierarchy VALUES(5, 2);
INSERT INTO Hierarchy VALUES(6, 2);
INSERT INTO Hierarchy VALUES(7, 2);
INSERT INTO Hierarchy VALUES(8, 5);
INSERT INTO Hierarchy VALUES(9, 5);