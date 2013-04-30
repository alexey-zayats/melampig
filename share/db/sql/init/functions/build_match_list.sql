-- build_match_list

CREATE OR REPLACE FUNCTION build_match_list(competition_id integer, style_id integer, cgroup_id integer, mat_id integer) RETURNS integer AS $$
DECLARE
	fight_id integer;
	wrestlers_num integer;
	ctour_num integer;
	ctour_id integer;
	cround_id integer;
	round_name varchar;
	numdiff integer;
	wrestler_id integer;
	offset_num integer;
	red_id integer;
	blue_id integer;
	fight_num integer;
	fights_left integer;
	fights_in_tour integer;
	tmp_id integer;
	tmp_t varchar;
	by_team boolean;
	wrestlers int[];
	circles int[];
	match_id integer;
BEGIN
	fight_id := 0;
	wrestlers_num := 0;
	wrestler_id := 0;
	red_id := 0;
	blue_id := 0;

	-- delete old fights
	DELETE FROM 
		fight_log
	USING 
		match as m,
		fight as f
	WHERE 
		m.competition = competition_id AND 
		m.style = style_id AND 
		m.cgroup = cgroup_id AND
		f.match = m.id AND
		fight_log.fight = f.id;

	DELETE FROM 
		fight_log 
	USING 
		fight 
	WHERE 
		fight.competition = competition_id AND 
		fight.style  = style_id AND
		fight.cround = cround_id AND  
		fight_log.fight = fight.id;
	
	DELETE FROM 
		fight
	USING 
		match as m 
	WHERE 
		m.competition = competition_id AND 
		m.style = style_id AND 
		m.cgroup = cgroup_id AND
		fight.match = m.id;

	DELETE FROM 
		match 
	WHERE 
		competition = competition_id AND
		style = style_id AND 
		cgroup = cgroup_id;
 
	-- get count of wrestler on 
	SELECT 
		INTO wrestlers_num count(team) 

	FROM 
		competition_team
	WHERE 
		competition = competition_id AND
		style = style_id AND
		cgroup = cgroup_id;

	IF NOT FOUND THEN
		RAISE EXCEPTION 'Not found team: competetition(%), style(%), group(%)', competition_id, style_id, cgroup_id;
	END IF;

	IF wrestlers_num < 6 THEN
		-- A(n,m) = n!/2(n-m)!
		-- fights to build
		-- fights_left = (wrestlers_num!) / ( 2 * (wrestlers_num-2)! );
		-- competitions circle to through out
		-- ctour_num := floor(fights_left/2);
		wrestlers := '{}';
		circles := '{}';
		FOR wrestler_id IN
			SELECT 
				team
			FROM 
				competition_team 
			WHERE 
				competition = competition_id AND
				style = style_id AND
				cgroup = cgroup_id
			ORDER BY
				sorder
		LOOP
			-- place wrestlers by order
			wrestlers := wrestlers + wrestler_id;
		END LOOP;

		FOR tmp_id IN SELECT id FROM circle ORDER BY id 
		LOOP
			circles := circles + tmp_id;
		END LOOP;

		fight_num := 0;

		IF wrestlers_num = 2 
		THEN
			INSERT INTO match (competition, style, cgroup, circle, num, mat, team_a, team_b) 
			VALUES (competition_id, style_id, cgroup_id, circles[1], 1, mat_id, wrestlers[1], wrestlers[2]) RETURNING id INTO match_id;
			perform build_match_fights(match_id);

			fight_num := 1;
		END IF;
		
		IF wrestlers_num = 3 
		THEN
			INSERT INTO match (competition, style, cgroup, circle, num, mat, team_a, team_b) 
			VALUES (competition_id, style_id, cgroup_id, circles[1], 1, mat_id, wrestlers[1], wrestlers[2]) RETURNING id INTO match_id;
			perform build_match_fights(match_id);

			INSERT INTO match (competition, style, cgroup, circle, num, mat, team_a, team_b) 
			VALUES (competition_id, style_id, cgroup_id, circles[2], 2, mat_id, wrestlers[3], wrestlers[1]) RETURNING id INTO match_id;
			perform build_match_fights(match_id);

			INSERT INTO match (competition, style, cgroup, circle, num, mat, team_a, team_b) 
			VALUES (competition_id, style_id, cgroup_id, circles[3], 3, mat_id, wrestlers[2], wrestlers[3]) RETURNING id INTO match_id;
			perform build_match_fights(match_id);

			fight_num := 3;
		END IF;

		IF wrestlers_num = 4 
		THEN
			INSERT INTO match (competition, style, cgroup, circle, num, mat, team_a, team_b) 
			VALUES (competition_id, style_id, cgroup_id, circles[1], 1, mat_id, wrestlers[1], wrestlers[2]) RETURNING id INTO match_id;
			perform build_match_fights(match_id);
			
			INSERT INTO match (competition, style, cgroup, circle, num, mat, team_a, team_b) 
			VALUES (competition_id, style_id, cgroup_id, circles[1], 2, mat_id, wrestlers[3], wrestlers[4]) RETURNING id INTO match_id;
			perform build_match_fights(match_id);

			INSERT INTO match (competition, style, cgroup, circle, num, mat, team_a, team_b) 
			VALUES (competition_id, style_id, cgroup_id, circles[2], 3, mat_id, wrestlers[1], wrestlers[3]) RETURNING id INTO match_id;
			perform build_match_fights(match_id);

			INSERT INTO match (competition, style, cgroup, circle, num, mat, team_a, team_b) 
			VALUES (competition_id, style_id, cgroup_id, circles[2], 4, mat_id, wrestlers[2], wrestlers[4]) RETURNING id INTO match_id;
			perform build_match_fights(match_id);

			INSERT INTO match (competition, style, cgroup, circle, num, mat, team_a, team_b) 
			VALUES (competition_id, style_id, cgroup_id, circles[3], 5, mat_id, wrestlers[1], wrestlers[4]) RETURNING id INTO match_id;
			perform build_match_fights(match_id);

			INSERT INTO match (competition, style, cgroup, circle, num, mat, team_a, team_b) 
			VALUES (competition_id, style_id, cgroup_id, circles[3], 6, mat_id, wrestlers[2], wrestlers[3]) RETURNING id INTO match_id;
			perform build_match_fights(match_id);

			fight_num := 6;
		END IF;

		IF wrestlers_num = 5 THEN
			INSERT INTO match (competition, style, cgroup, circle, num, mat, team_a, team_b) 
			VALUES (competition_id, style_id, cgroup_id, circles[1], 1, mat_id, wrestlers[1], wrestlers[2]) RETURNING id INTO match_id;
			perform build_match_fights(match_id);

			INSERT INTO match (competition, style, cgroup, circle, num, mat, team_a, team_b) 
			VALUES (competition_id, style_id, cgroup_id, circles[1], 2, mat_id, wrestlers[3], wrestlers[4]) RETURNING id INTO match_id;
			perform build_match_fights(match_id);

			INSERT INTO match (competition, style, cgroup, circle, num, mat, team_a, team_b) 
			VALUES (competition_id, style_id, cgroup_id, circles[2], 3, mat_id, wrestlers[5], wrestlers[1]) RETURNING id INTO match_id;
			perform build_match_fights(match_id);

			INSERT INTO match (competition, style, cgroup, circle, num, mat, team_a, team_b) 
			VALUES (competition_id, style_id, cgroup_id, circles[2], 4, mat_id, wrestlers[2], wrestlers[3]) RETURNING id INTO match_id;
			perform build_match_fights(match_id);

			INSERT INTO match (competition, style, cgroup, circle, num, mat, team_a, team_b) 
			VALUES (competition_id, style_id, cgroup_id, circles[3], 5, mat_id, wrestlers[4], wrestlers[1]) RETURNING id INTO match_id;
			perform build_match_fights(match_id);

			INSERT INTO match (competition, style, cgroup, circle, num, mat, team_a, team_b) 
			VALUES (competition_id, style_id, cgroup_id, circles[3], 6, mat_id, wrestlers[5], wrestlers[2]) RETURNING id INTO match_id;
			perform build_match_fights(match_id);

			INSERT INTO match (competition, style, cgroup, circle, num, mat, team_a, team_b) 
			VALUES (competition_id, style_id, cgroup_id, circles[4], 7, mat_id, wrestlers[3], wrestlers[1]) RETURNING id INTO match_id;
			perform build_match_fights(match_id);

			INSERT INTO match (competition, style, cgroup, circle, num, mat, team_a, team_b) 
			VALUES (competition_id, style_id, cgroup_id, circles[4], 8, mat_id, wrestlers[4], wrestlers[5]) RETURNING id INTO match_id;
			perform build_match_fights(match_id);

			INSERT INTO match (competition, style, cgroup, circle, num, mat, team_a, team_b) 
			VALUES (competition_id, style_id, cgroup_id, circles[5], 9, mat_id, wrestlers[2], wrestlers[4]) RETURNING id INTO match_id;
			perform build_match_fights(match_id);

			INSERT INTO match (competition, style, cgroup, circle, num, mat, team_a, team_b) 
			VALUES (competition_id, style_id, cgroup_id, circles[5], 10, mat_id, wrestlers[3], wrestlers[5]) RETURNING id INTO match_id;
			perform build_match_fights(match_id);

			fight_num := 10;
		END IF;
	ELSE
		-- find nearest competition tour
		SELECT INTO ctour_id, ctour_num, tmp_t id, num, title FROM ctour WHERE num < wrestlers_num ORDER BY num desc LIMIT 1;
		IF NOT FOUND THEN
			RAISE EXCEPTION 'Not found competition tour less of num(%)', wrestlers_num;
		END IF;

		-- get name of first round 
		numdiff := wrestlers_num - ctour_num;
		IF ctour_num = numdiff THEN
			round_name := 'selection';
		ELSE
			round_name := 'qualification';
		END IF;

		SELECT INTO cround_id, tmp_t id, title FROM cround WHERE name = round_name LIMIT 1;
		IF NOT FOUND THEN
			RAISE EXCEPTION 'Not found competition round by name(%)', round_name;
		END IF;

		offset_num := 0;
		IF ctour_num = wrestlers_num THEN offset_num := 0; 
		ELSE offset_num := (ctour_num*2) - wrestlers_num;
		END IF; 

		-- find required fights num 
		fights_left := wrestlers_num - ctour_num;

		FOR fights_in_tour IN
			SELECT num FROM ctour WHERE num < ctour_num ORDER BY num DESC
		LOOP
			fights_left := fights_left + fights_in_tour;
		END LOOP;

		-- create fights for first round
		fight_num := 0;
		FOR wrestler_id IN
			SELECT 
				team
			FROM 
				competition_team
			WHERE 
				competition = competition_id AND
				style = style_id AND
				cgroup = cgroup_id
			ORDER BY
				sorder
			OFFSET
				offset_num
		LOOP
			IF red_id = 0 THEN 
				red_id := wrestler_id; 
				CONTINUE;
			END IF;

			IF red_id > 0 AND blue_id = 0 THEN
				blue_id := wrestler_id; 
			END IF;

			IF red_id > 0 AND blue_id > 0 THEN

				fight_num := fight_num + 1;
				fights_left := fights_left - 1;
			
				INSERT INTO match (competition, style, cgroup_id, cround, ctour, num, mat, team_a, team_b)
				VALUES (competition_id, style_id, cgroup_id, cround_id, ctour_id, fight_num, mat_id, red_id, blue_id)
					RETURNING id INTO match_id;

				perform build_match_fights(match_id);
			
				red_id := 0;
				blue_id := 0;
			END IF;
		END LOOP;

		-- get next round
		IF round_name = 'qualification' THEN
			SELECT INTO cround_id, tmp_t id, title FROM cround WHERE name = 'selection' ORDER BY name DESC LIMIT 1;
			IF NOT FOUND THEN
				RAISE EXCEPTION 'Not found competition round for ''selection''';
			END IF;
		END IF;

		-- get next tour
		SELECT INTO ctour_id, ctour_num, tmp_t id, num, title FROM ctour WHERE num < ctour_num ORDER BY num desc LIMIT 1;
		IF NOT FOUND THEN
			RAISE EXCEPTION 'Not found competition tour less of num(%)', ctour_num;
		END IF;

		-- create fights with free wrestlers for next round and 
		FOR wrestler_id IN
			SELECT 
				team
			FROM 
				competition_team 
			WHERE 
				competition = competition_id AND
				style = style_id AND
				cgroup = cgroup_id
			ORDER BY
				sorder
			LIMIT
				offset_num
		LOOP
			IF red_id = 0 THEN 
				red_id := wrestler_id; 
				CONTINUE;
			END IF;
			IF red_id > 0 AND blue_id = 0 THEN
				blue_id := wrestler_id; 
			END IF;
			IF red_id > 0 AND blue_id > 0 THEN
				fight_num := fight_num + 1;
				fights_left := fights_left - 1;

				INSERT INTO match (competition, style, cgroup, cround, ctour, num, mat, team_a, team_b)
				VALUES (competition_id, style_id, cgroup_id, cround_id, ctour_id, fight_num, mat_id, red_id, blue_id)
					RETURNING id INTO match_id;

				perform build_match_fights(match_id);
		
				red_id := 0;
				blue_id := 0;
			END IF;
		END LOOP;

		IF red_id > 0 THEN 
			fight_num := fight_num + 1;
			fights_left := fights_left - 1;

			INSERT INTO match (competition, style, cgroup, cround, ctour, num, team_a, team_b)
			VALUES (competition_id, style_id, cgroup_id, cround_id, ctour_id, fight_num, mat_id, red_id)
				RETURNING id INTO match_id;

			perform build_match_fights(match_id);
			red_id := 0;
		END IF;

		-- create empty fights with free wrestlers for next round and 
		WHILE fights_left > 0 
		LOOP
			fight_num := fight_num + 1;
		
			SELECT INTO ctour_id, ctour_num, tmp_t id, num, title FROM ctour WHERE num <= fights_left ORDER BY num DESC LIMIT 1;

			IF ctour_num = 1 THEN
				SELECT INTO cround_id, tmp_t id, title FROM cround WHERE name = 'final'; 
			END IF;

			INSERT INTO match (competition, style, cgroup, cround, ctour, num, mat)
			VALUES (competition_id, style_id, cgroup_id, cround_id, ctour_id, fight_num, mat_id)
				RETURNING id INTO match_id;

			perform build_match_fights(match_id);
		
			fights_left := fights_left - 1;
		END LOOP;
	END IF;
	

	RETURN fight_num;
END
$$ language plpgsql;
