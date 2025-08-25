-- 姓名：田永铭
-- 学号：221900180
-- 提交前请确保本次实验独立完成，若有参考请注明并致谢。

-- 1. 有多少物种species的描述description中含有单词“this”？___________________________________________________________________________________________________________________________________________________________________________________________________________
-- BEGIN Q1
SELECT COUNT(*) AS speciesCount
FROM species
WHERE description LIKE '%this%';
-- END Q1

-- 2. 玩家player‘Cook’将与玩家player‘Hughes’作战。显示他们的用户名username和他们各自拥有的Phonemon的总能量。 ____________________________________________________________________________________________________________________________________________________________________________________________________________
-- BEGIN Q2
SELECT p.username,(SELECT SUM(power) FROM phonemon WHERE phonemon.player = p.id) AS totalPhonemonPower
FROM player p
WHERE p.username IN ('Cook', 'Hughes');
-- END Q2

-- 3. 每一个队伍team有多少名成员player？按照玩家数量降序列出队伍名称title和玩家数量。____________________________________________________________________________________________________________________________________________________________________________________________________________
-- BEGIN Q3
SELECT t.title, (SELECT COUNT(*) FROM player WHERE player.team = t.id) AS numberOfPlayers
FROM team t
ORDER BY numberOfPlayers DESC;
-- END Q3

-- 4. 哪些物种species具有类型type‘grass’？____________________________________________________________________________________________________________________________________________________________________________________________________________
-- BEGIN Q4
SELECT s.id AS idSpecies, s.title 
FROM species s, type t
WHERE t.title = 'grass' and (t.id = s.type1 or t.id = s.type2);
-- END Q4

-- 5. 列出从未购买过食物food的玩家player。____________________________________________________________________________________________________________________________________________________________________________________________________________
-- BEGIN Q5
SELECT p.id AS idPlayer, p.username
FROM player p
WHERE p.id NOT IN (
	SELECT p2.id
    FROM player p2, purchase pur, item i
    WHERE pur.player = p2.id and pur.item = i.id and i.type = 'F'
);
-- END Q5

-- 6. 游戏中的每个玩家player具有特定的等级level。以金额降序列出每一特定等级以及该等级的所有玩家在购买上花费的总金额。____________________________________________________________________________________________________________________________________________________________________________________________________________
-- BEGIN Q6
SELECT p.level, SUM(i.price * pur.quantity) AS totalAmountSpentByAllPlayersAtLevel
FROM player p, purchase pur, item i
WHERE p.id = pur.player and pur.item = i.id
GROUP BY p.level
ORDER BY totalAmountSpentByAllPlayersAtLevel DESC;
-- END Q6

-- 7. 什么物品item被购买次数最多？如有并列，列出所有购买次数最多的物品。____________________________________________________________________________________________________________________________________________________________________________________________________________
-- BEGIN Q7
SELECT i.id AS item, i.title, COUNT(pur.id) AS numTimesPurchased
FROM purchase pur, item i
WHERE pur.item = i.id
GROUP BY i.id, i.title
HAVING  COUNT(pur.id) = (
        SELECT MAX(times_count)
        FROM (
            SELECT COUNT(pur2.id) AS times_count
            FROM purchase pur2
            GROUP BY pur2.item
        ) AS max_times
    );               
-- END Q7

-- 8. 找到可获取的食物的数量，和购买所有种类食物至少各一次的玩家。____________________________________________________________________________________________________________________________________________________________________________________________________________
-- BEGIN Q8
SELECT p.id AS playerID, p.username, COUNT(DISTINCT i.id) AS numberDistinctFoodItemsPurchased
FROM player p, purchase pur, item i
WHERE p.id = pur.player and pur.item = i.id and i.type = 'F'
GROUP BY p.id, p.username
HAVING COUNT(DISTINCT i.id) = (
    SELECT COUNT(DISTINCT i2.id)
    FROM item i2
    WHERE type = 'F'
);
-- END Q8

-- 9. 将距离最近的两个Phonemon之间的欧氏距离称为X。计算相互之间距离为X的Phonemon对的数量。____________________________________________________________________________________________________________________________________________________________________________________________________________
-- BEGIN Q9
SELECT COUNT(*)  AS numberOfPhonemonPairs,(
         SELECT MIN(ROUND(SQRT(POW(p1.latitude - p2.latitude, 2) + POW(p1.longitude - p2.longitude, 2)) * 100,2))
         FROM phonemon p1, phonemon p2
         WHERE p1.id < p2.id
       ) AS distanceX
FROM phonemon p3, phonemon p4
WHERE p3.id < p4.id and (ROUND(SQRT(POW(p3.latitude - p4.latitude, 2) + POW(p3.longitude - p4.longitude, 2)) * 100,2)) = (
	SELECT MIN(ROUND(SQRT(POW(p1.latitude - p2.latitude, 2) + POW(p1.longitude - p2.longitude, 2)) * 100,2))
	FROM phonemon p1, phonemon p2
	WHERE p1.id < p2.id
    );
-- END Q9

-- 10. 一些玩家player热衷于某种特定类型type的Phonemon。列出捕捉了任一特定类型type中每一物种species至少各一个Phonemon的玩家的名称以及该类型的名称。____________________________________________________________________________________________________________________________________________________________________________________________________________
-- BEGIN Q10
SELECT p.username, t.title as typeTitle
FROM player p, type t, phonemon pho, species s
WHERE (t.id = s.type1 or t.id = s.type2) and pho.player = p.id and pho.species = s.id
GROUP BY username, typeTitle
HAVING COUNT(distinct s.id) = (
	SELECT COUNT(*) 
    FROM species s2,type t2
	WHERE (s2.type1 = t2.id or s2.type2 = t2.id) and t2.title = t.title
    );
-- END Q10