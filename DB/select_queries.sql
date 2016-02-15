-- Query 1: selects the name from members table and checks if it is alive and the name starts with Tim

SELECT name
FROM members
WHERE stillalive='Y' AND name LIKE 'Tim%';

-- Query 2: firstly, it selects the band id from bands table that corresponds to Iron Maiden;
--          afterwards, it selects all the member ids from memberof table that belong to the band;
--          lastly, it takes their names from the members table which have their ids as the ones retrieved from the step above


SELECT name
FROM members
WHERE mid IN (SELECT mid
	     FROM memberof
	     WHERE bid = (SELECT bid
			  FROM bands
			  WHERE name='Iron Maiden') AND endyear IS NULL);


-- Query 3: selects all members' names and checks which band names are in the results retrieved

SELECT name
FROM bands
WHERE name IN (select name
	    from members
	    );


-- Query 4: takes from the memberof table all the tuples that have instrument equal to bass and counts the tuples retrieved

SELECT COUNT(*) AS Bass_Player_Count
FROM memberof
WHERE instrument='bass';


-- Query 5: ???

SELECT DISTINCT m.name AS Drummer_Name, r.title AS Release_Name, r.year AS Year
FROM members m
JOIN memberof mo ON m.mid = mo.mid
JOIN releases r ON r.bid = mo.bid
WHERE mo.instrument='drums'
ORDER BY m.name,year;


-- Query 6: firstly, it joins releases on band id, afterwards joining songs on the release id;
--          then, we check which tuples have cdbonus equal to yes and the rating is either null or greater or equal than 5,
--          finally grouping them by band name and ordering it by band name as well;

select b.name as Band_Name, count(cdbonus) as Bonus_Num
from bands b
join releases r on b.bid = r.bid
join songs s on s.rid = r.rid
where s.cdbonus='Y' and (r.rating >= 5 or r.rating is null)
group by Band_Name
order by Band_Name;


-- Query 7: we get all releases' years (which are not null) and select the bands' names that have all releases 
--          launched after 1999

SELECT b.name
FROM bands b
WHERE 1999 < ALL (select year
		  from releases r
		  where b.bid=r.bid and year is not null);

-- Query 8: we get all realeases' years (not null) and then select bands' names that have at least one release launched since 2011

SELECT b.name
FROM bands b
WHERE 2011 <= ANY (select year
		  from releases r
		  where b.bid=r.bid and year is not null);				  


   