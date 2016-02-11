-- Query 1

SELECT name
FROM members
WHERE stillalive='Y' AND name LIKE 'Tim%';

-- Query 2

SELECT name
FROM members
WHERE mid IN (SELECT mid
	     FROM memberof
	     WHERE bid = (SELECT bid
			  FROM bands
			  WHERE name='Iron Maiden') AND endyear IS NULL);

-- Query 3

SELECT name
FROM bands
WHERE name IN (select name
	    from members
	    );

-- Query 4

SELECT COUNT(*) AS Bass_Player_Count
FROM memberof
WHERE instrument='bass';

-- Query 5

SELECT DISTINCT m.name AS Drummer_Name, r.title AS Release_Name, r.year AS Year
FROM members m, releases r, memberof mo
WHERE mo.bid=r.bid and r.bid = m.mid and mo.instrument='drums'
ORDER BY m.name,year;

-- Query 6

select b.name as Band_Name, count(cdbonus) as Bonus_Num
from bands b, releases r, songs s
where b.bid=r.bid and s.rid=r.rid and s.cdbonus='Y' and (r.rating >= 5 or r.rating is null)
group by Band_Name
order by Band_Name;

-- Query 7

SELECT b.name
FROM bands b
WHERE 1999 < ALL (select year
		  from releases r
		  where b.bid=r.bid and year is not null);

-- Query 8

SELECT b.name
FROM bands b
WHERE 2011 <= ANY (select year
		  from releases r
		  where b.bid=r.bid and year is not null);				  




   