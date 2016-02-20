-- Query 1: SELECTs the name FROM members table and checks if it is alive and the name starts with Tim

SELECT name
FROM members
WHERE stillalive='Y' AND name LIKE 'Tim%';

-- Query 2: firstly, it SELECTs the band id FROM bands table that corresponds to Iron Maiden;
--          afterwards, it SELECTs all the member ids FROM memberof table that belong to the band;
--          lastly, it takes their names FROM the members table which have their ids as the ones retrieved FROM the step above


SELECT name
FROM members
WHERE mid IN (SELECT mid
	     FROM memberof
	     WHERE bid = (SELECT bid
			  FROM bands
			  WHERE name='Iron Maiden') AND endyear IS NULL);


-- Query 3: SELECTs all members' names and checks which band names are in the results retrieved

SELECT name
FROM bands
WHERE name IN (SELECT name
	    FROM members
	    );


-- Query 4: takes FROM the memberof table all the tuples that have instrument equal to bass and counts the tuples retrieved

SELECT COUNT(DISTINCT mid) AS Bass_Player_Count
FROM memberof
WHERE instrument='bass';


-- Query 5: gets all drummers from the cartesian product of memberof, releases and members, and then checks if
--          the release was launched while the drummer was part of the band; lastly, we order at first by the 
--          drummer's name (such that all drummer's releases are grouped together), and then by year; a group by
--          clause on the drummer name could not be added here as we have no aggregate functions to apply on the other
--          2 columns in the select clause (Release_Name and Year)

SELECT members.name AS Drummer_Name, releases.title AS Release_Name, releases.year AS Year
FROM members,releases,memberof
WHERE releases.bid= memberof.bid AND members.mid = memberof.mid AND memberof.instrument='drums' AND
((memberof.startyear IS NULL AND memberof.endyear IS NULL) OR
(memberof.startyear <= releases.year AND  memberof.endyear is NULL) OR
(memberof.startyear IS NULL AND  releases.year <= memberof.endyear) OR
(memberof.startyear <= releases.year AND  releases.year <= memberof.endyear))
ORDER BY Drummer_Name, Year;


-- Query 6: firstly, it joins releases on band id, afterwards joining songs on the release id;
--          then, we check which tuples have cdbonus equal to yes and the rating is either null or greater or equal than 5,
--          finally grouping them by band name and ordering it by band name as well;

SELECT b.name AS Band_Name, count(cdbonus) AS Bonus_Num
FROM bands b
JOIN releases r ON b.bid = r.bid
JOIN songs s ON s.rid = r.rid
WHERE s.cdbonus='Y' AND (r.rating >= 5 OR r.rating IS NULL)
GROUP BY Band_Name
ORDER BY Band_Name;


-- Query 7: we get all releases' years (which are not null) and SELECT the bands' names that have all releases 
--          launched after 1999 (excluding 1999)

SELECT DISTINCT b.name
FROM bands b
WHERE 1999 < ALL (SELECT year
		  FROM releases r
		  WHERE b.bid=r.bid AND year IS NOT NULL);

-- Query 8: we get all realeases' years (not null) and then SELECT bands' names that have at least one release launched since 2011
--          (including 2011)

SELECT DISTINCT b.name
FROM bands b
WHERE 2011 <= ANY (SELECT year
		  FROM releases r
		  WHERE b.bid=r.bid AND year IS NOT NULL);