-- Creates table bands with primary key bid

CREATE TABLE bands (
    bid INTEGER,
    name VARCHAR(50),
    country VARCHAR(20),
    webpage VARCHAR(120),
    PRIMARY KEY (bid)
);

-- Creates table releases with primary key rid and bid as a foreign key referencing to bands' bid

CREATE TABLE releases (
    rid INTEGER,
    bid INTEGER,
    title VARCHAR(120),
    year INTEGER,
    type VARCHAR(10),
    rating INTEGER,
    PRIMARY KEY (rid),
    FOREIGN KEY (bid) REFERENCES bands(bid)
);

-- Here we need a composite primary key (title and rid) as it will be a unique identifier in the DB, with a foreign key
-- referencing to releases' rid

CREATE TABLE songs (
    title VARCHAR(120),
    rid INTEGER,
    cdbonus CHAR(1),
    PRIMARY KEY (title, rid),
    FOREIGN KEY (rid) REFERENCES releases(rid)
);

-- Creates members table with mid as a primary key

CREATE TABLE members (
    mid INTEGER,
    name VARCHAR(50),
    stillalive CHAR(1),
    PRIMARY KEY (mid)
);

-- Creates table memberof with no primary key as it is a relationship between two tables in the DB;
-- it has 2 foreign keys to the tables that it's referencing, bid to bands and mid to members

CREATE TABLE memberof (
    mid INTEGER,
    bid INTEGER,
    startyear INTEGER,
    endyear INTEGER,
    instrument VARCHAR(15),
    FOREIGN KEY (bid) REFERENCES bands(bid),
    FOREIGN KEY (mid) REFERENCES members(mid)
);