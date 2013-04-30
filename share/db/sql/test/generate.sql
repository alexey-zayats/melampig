
-- DROP FUNCTION test.generate_competition(byteam boolean, wrestler_num integer);

/*
	Функция генерация тестового соревнования.
	Входные параметры:
	1. is_byteam boolean - признак генерации командного соревнования.
	2. team_num integer - количество команд в  соревновании.
	3. wrestler_num - количество борцов в весовой категории
	4. weight_num integer - количество весовых категорий в соревновании
	
	Описание алгоритма работы
	1. Получение данных(случайным образом из справочников) для создания соревнования
		1. Возрастная категория
		2. Тип соревнования
		3. Страна/Городо - место проведения соревнования
		4. Стили соревнования: вольная, греко-римская, женская борьба
	2. Создать соревнование
	3. Получить K весовых категорий, случайным образом из справочника весовых категорий, где K - weight_num
	4. Создать M команд, случайным образом из справочника географий, где M - team_num
	5. Для каждой команды в каждой весовой категории:
		1. Создать борца
		2. Связать борца, команду и соревнование	
*/	

CREATE OR REPLACE FUNCTION test.generate_competition(in_title varchar, is_byteam boolean, team_num integer, wrestler_num integer, weight_num integer) RETURNS integer AS $$
DECLARE
	c_id integer;
	c_title varchar;
	c_age integer;
	c_type integer;
	c_geo integer;
	c_style integer;

	fio record;
	wrestler_id integer;

	male_id integer;
	w_geo integer;
	w_rang integer;

	weight record;
	weights hstore;
	weight_count integer;
	
	i integer;
	j integer;
	
	teams hstore;
	team record;
	team_id integer;

BEGIN

	-- get wrestlers age by random
	SELECT INTO c_age id FROM age ORDER BY random() LIMIT 1;

	-- get competition type by random
	SELECT INTO c_type id FROM comptype ORDER BY random() LIMIT 1;
	
	-- get competition geo placement by random
	SELECT INTO c_geo id FROM geo WHERE level > 1 ORDER BY random() LIMIT 1;

	-- get competition style by random
	SELECT INTO c_style id FROM style ORDER BY random() LIMIT 1;

	SELECT INTO male_id id FROM gender WHERE name = 'male';
	
	-- формируем название соревнования
	IF is_byteam = TRUE 
	THEN
		c_title := in_title || ': командное';
	ELSE
		c_title := in_title || ': личное';
	END IF;
	
	-- создаем соревнование
	INSERT INTO competition (title, start, stop, age, comptype, byteam, geo)
		VALUES (c_title, current_date, current_date + interval '2 days', c_age, c_type, is_byteam, c_geo) 
		RETURNING id INTO c_id;
	
	INSERT INTO competition_style (competition, style) VALUES (c_id, c_style); 

	-- получем общее кол-во весовых категорий
	SELECT INTO weight_count count(id) FROM weight;
	-- проверяем, не запрошено ли кол-во весовых категорий больше чем есть в справочнике
	IF weight_count < weight_num THEN
		-- выводим ошибку, если кол-во, запрошенных к генерации, весовых категорий больше, чем есть в справочнике
		raise exception 'Required weight num is biggest then weights count we have. Decrease weight num for test competition and try again';
	END IF;

	-- объявляем хеш-массив весовых категорий
	weights := '';
	
	-- получем весовые категории в случайном порядке из справочника
	i := 0;
	WHILE i < weight_num 
	LOOP
		SELECT INTO weight cast(id as text) as id, cast(title as integer) as title FROM weight ORDER BY random() LIMIT 1;
		IF TRUE = exist(weights, weight.id) THEN CONTINUE; END IF;
		weights := weights || cast( weight.id || '=>' || weight.title  as hstore);  

		INSERT INTO competition_weight (competition, weight) VALUES (c_id, cast(weight.id as integer)); 
		i := i + 1;
	END LOOP;
			
	-- raise notice 'weight num: %', array_length(akeys(weights), 1);
	
	-- создаем команды
	teams := '';
	FOR team IN 
		SELECT id, name, title FROM geo ORDER BY random() LIMIT team_num
	LOOP
		INSERT INTO team (name, title, geo) VALUES (team.name, team.title, team.id) RETURNING id INTO team_id;
		INSERT INTO competition_team (competition, team, style) VALUES (c_id, team_id, c_style);

		IF TRUE = exist(teams, cast(team_id as text)) THEN CONTINUE; END IF;
		teams := teams || cast( team_id || '=>' || team.id  as hstore);  
	END LOOP; 
	
	IF is_byteam 
	THEN
		FOR team IN SELECT * FROM each(teams) 
		LOOP
			raise notice 'team: % => %', team.key, team.value;
			FOR weight IN SELECT * FROM each(weights) ORDER BY 1
			LOOP
				raise notice 'weight: % => %', weight.key, weight.value;
				FOR fio IN SELECT * FROM test.fio ORDER BY random() LIMIT 1
				LOOP
					SELECT INTO w_rang id FROM rang WHERE trail ~ 'root.wrestler.*{1}' ORDER BY random() LIMIT 1;

					INSERT INTO wrestler (firstname, lastname, gender, rang, geo) 
						VALUES (fio.firstname, fio.lastname, male_id, w_rang, cast(team.value as integer)) 
						RETURNING id INTO wrestler_id;

					-- raise notice 'wrestler: % %', fio.lastname, fio.firstname;
					-- RETURN NEXT fio.lastname || ' - ' || fio.firstname;

					INSERT INTO competition_wrestler (competition, team, wrestler, style, weight) 
						VALUES ( c_id, cast(team.key as integer), wrestler_id, c_style, cast(weight.key as integer) );
				END LOOP;
			END LOOP;
		END LOOP;
	ELSE
		FOR weight IN SELECT * FROM each(weights) ORDER BY 1
		LOOP
			raise notice 'weight: % => %', weight.key, weight.value;
			FOR fio IN SELECT * FROM test.fio ORDER BY random() LIMIT wrestler_num
			LOOP
				SELECT INTO team * FROM each(teams) ORDER BY random() LIMIT 1; 
				SELECT INTO w_rang id FROM rang WHERE trail ~ 'root.wrestler.*{1}' ORDER BY random() LIMIT 1;

				INSERT INTO wrestler (firstname, lastname, gender, rang, geo) 
					VALUES (fio.firstname, fio.lastname, male_id, w_rang, cast(team.value as integer)) 
						RETURNING id INTO wrestler_id;

				-- raise notice 'wrestler: % %', fio.lastname, fio.firstname;
				-- RETURN NEXT fio.lastname || ' - ' || fio.firstname;

				INSERT INTO competition_wrestler (competition, team, wrestler, style, weight) 
					VALUES ( c_id, cast(team.key as integer), wrestler_id, c_style, cast(weight.key as integer) );
			END LOOP;
		END LOOP;
	END IF;
		
	RETURN c_id;
END 
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION test.remove_competition(c_id integer) RETURNS INTEGER AS $$
DECLARE
	arow record;
BEGIN
	DELETE FROM fight_log USING fight as f WHERE f.competition = c_id AND fight_log.fight = f.id;
	DELETE FROM fight WHERE competition = c_id;
	
	FOR arow IN SELECT wrestler FROM competition_wrestler WHERE competition = c_id LOOP
		DELETE FROM competition_wrestler WHERE competition = c_id AND wrestler = arow.wrestler;
		DELETE FROM wrestler WHERE id = arow.wrestler;
	END LOOP;

	DELETE FROM competition_weight WHERE competition = c_id;
	DELETE FROM competition_style WHERE competition = c_id;
	DELETE FROM competition_team WHERE competition = c_id;
	DELETE FROM competition WHERE id = c_id;
	RETURN c_id; 
END
$$ LANGUAGE plpgsql;

