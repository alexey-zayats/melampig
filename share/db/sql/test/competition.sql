-- test.generate_competition(in_title varchar, is_byteam boolean, team_num integer, wrestler_num integer, weight_num integer) 

-- personal
-- 8
select test.generate_competition('8 борцов', false, 5, 8, 5);
-- 16
select test.generate_competition('16 борцов', false, 5, 16, 5);
-- 32
select test.generate_competition('32 борца', false, 5, 32, 5);
-- 64
select test.generate_competition('64 борца', false, 5, 32, 5);
-- 128
select test.generate_competition('128 борцов', false, 5, 32, 5);

-- by team
select test.generate_competition('3 команды', true, 3,  8, 7);
select test.generate_competition('4 команды', true, 4,  8, 7);
select test.generate_competition('5 команд',  true, 5,  8, 7);
select test.generate_competition('6 команд',  true, 6,  8, 7);
select test.generate_competition('7 команд',  true, 7,  8, 7);
select test.generate_competition('8 команд',  true, 8,  8, 7);
select test.generate_competition('9 команд',  true, 9,  8, 7);
select test.generate_competition('10 команд', true, 10, 8, 7);

