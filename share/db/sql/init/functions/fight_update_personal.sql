
CREATE OR REPLACE FUNCTION fight_update_personal() RETURNS TRIGGER AS $$
DECLARE
	half_final_id integer;
	wrestler_id integer;
	fight_num integer;
	fight_id integer;
	fight_red integer;
	fight_blue integer;
	final_tour integer; -- once
	fightrec record;
	cround_id integer;
	qualification_round_id integer;
	selection_round_id integer;
	consolation_round_id_1 integer;
	consolation_round_id_2 integer;
	final_round_id integer;
	tmp integer;
	winner_id integer;
	looser_id integer;
	winner_points integer;
	looser_points integer;
	y integer;
	z integer;
	wcount integer;
	fcount integer; -- once
	fid int[];
	start_offset integer;
	fpos integer;
	
	byteam 			boolean;
BEGIN

	SELECT INTO byteam c.byteam FROM competition as c WHERE c.id = NEW.competition;
	
	
	IF NEW.winner IS NULL THEN RETURN NEW; END IF;
	
	-- on first, we need to store points for classify ranking

	-- clear old values

	IF OLD.winner IS NOT NULL 
	THEN
		winner_id := OLD.winner;

		-- take old points
		IF OLD.winner = OLD.blue 
		THEN
			looser_id     := OLD.red;
			looser_points := sum_intarray(OLD.red_score);
			winner_points    := sum_intarray(OLD.blue_score);
		ELSE
			looser_id     := OLD.blue;
			looser_points := sum_intarray(OLD.blue_score);
			winner_points    := sum_intarray(OLD.red_score);
		END IF;
			
		-- remove old points for winner
		UPDATE 
			competition_wrestler 
		SET 
			classify_points    = classify_points   - c.win,
			tech_points_win    = tech_points_win   - winner_points,
			tech_points_loose  = tech_points_loose - looser_points,
			vt = vt - case when c.name = 'vt' then 1 else 0 end,
			va = va - case when c.name = 'va' then 1 else 0 end,
			vb = vb - case when c.name = 'vb' then 1 else 0 end,
			vf = vf - case when c.name = 'vf' then 1 else 0 end,
			ev = ev - case when c.name = 'ev' then 1 else 0 end,
			ex = ex - case when c.name = 'ex' then 1 else 0 end,
			st = st - case when c.name = 'st' then 1 else 0 end,
			sp = sp - case when c.name = 'sp' then 1 else 0 end,
			pp = pp - case when c.name = 'pp' then 1 else 0 end,
			po = po - case when c.name = 'po' then 1 else 0 end,
			e2 = e2 - case when c.name = 'e2' then 1 else 0 end
		FROM
			classify as c
		WHERE
			wrestler    = winner_id AND
			competition = OLD.competition AND 
			style       = OLD.style AND
			weight      = OLD.weight AND
			c.id        = OLD.classify;

		-- remove old poins for looser
		UPDATE 
			competition_wrestler 
		SET 
			classify_points    = classify_points   - c.fail
			,tech_points_win   = tech_points_win   - looser_points
			,tech_points_loose = tech_points_loose - winner_points
		FROM
			classify as c
		WHERE
			wrestler    = looser_id AND
			competition = OLD.competition AND 
			style       = OLD.style AND
			weight      = OLD.weight AND
			c.id        = OLD.classify;	
	END IF;
		
	winner_id := NEW.winner;

	-- take looser and tech points for win and loose
	IF NEW.winner = NEW.blue 
	THEN
		looser_id       := NEW.red;
		looser_points   := sum_intarray(NEW.red_score);
		winner_points   := sum_intarray(NEW.blue_score);
	ELSE
		looser_id       := NEW.blue;
		looser_points   := sum_intarray(NEW.blue_score);
		winner_points   := sum_intarray(NEW.red_score);
	END IF;
		
		
	-- store points for fight winner 
	UPDATE 
		competition_wrestler 
	SET 
		classify_points   = classify_points   + c.win,
		tech_points_win   = tech_points_win   + winner_points,
		tech_points_loose = tech_points_loose + looser_points,
		vt = vt + case when c.name = 'vt' then 1 else 0 end,
		va = va + case when c.name = 'va' then 1 else 0 end,
		vb = vb + case when c.name = 'vb' then 1 else 0 end,
		vf = vf + case when c.name = 'vf' then 1 else 0 end,
		ev = ev + case when c.name = 'ev' then 1 else 0 end,
		ex = ex + case when c.name = 'ex' then 1 else 0 end,
		st = st + case when c.name = 'st' then 1 else 0 end,
		sp = sp + case when c.name = 'sp' then 1 else 0 end,
		pp = pp + case when c.name = 'pp' then 1 else 0 end,
		po = po + case when c.name = 'po' then 1 else 0 end,
		e2 = e2 + case when c.name = 'e2' then 1 else 0 end
	FROM
		classify as c
	WHERE
		wrestler = winner_id AND
		competition = NEW.competition AND 
		style = NEW.style AND
		weight = NEW.weight AND
		c.id = NEW.classify;

	-- store points for fight looser
	UPDATE 
		competition_wrestler 
	SET 
		classify_points = classify_points + c.fail,
		tech_points_loose = tech_points_loose + winner_points,
		tech_points_win = tech_points_win + looser_points
	FROM
		classify as c
	WHERE
		wrestler = looser_id AND
		competition = NEW.competition AND 
		style = NEW.style AND
		weight = NEW.weight AND
		c.id = NEW.classify;


IF byteam = TRUE THEN RETURN NEW; END IF;

	-- from now we try to fill in free fights with current winner

	-- fetch id's for competition tour
	SELECT INTO half_final_id id FROM ctour WHERE num = 2;
	SELECT INTO final_tour    id FROM ctour WHERE num = 1;
	
	-- fetch id's for competition round 
	SELECT INTO qualification_round_id id FROM cround WHERE name = 'qualification';
	SELECT INTO selection_round_id     id FROM cround WHERE name = 'selection';
	SELECT INTO consolation_round_id_1 id FROM cround WHERE name = 'consolation-1';
	SELECT INTO consolation_round_id_2 id FROM cround WHERE name = 'consolation-2';
	SELECT INTO final_round_id         id FROM cround WHERE name = 'final';

	-- if we on final round nothing to do
	IF OLD.cround = final_round_id THEN RETURN NEW; END IF;

	-- clear old try
	UPDATE 
		fight 
	SET 
		red = NULL 
	WHERE 
		competition = OLD.competition
		AND style = OLD.style
		AND weight = OLD.weight
		AND winner IS NULL
		AND red = NEW.winner;

	UPDATE 
		fight 
	SET 
		blue = NULL 
	WHERE 
		competition = OLD.competition
		AND style = OLD.style
		AND weight = OLD.weight
		AND winner IS NULL
		AND blue = NEW.winner;

	IF 
		   NEW.cround = qualification_round_id
		OR NEW.cround = selection_round_id 
	THEN
		SELECT 
			INTO wcount 
			count(id) 
		FROM 
			competition_wrestler
		WHERE 
			competition = OLD.competition
			AND style = OLD.style
			AND weight = OLD.weight;

		SELECT INTO fid array( 
			SELECT 
				id 
			FROM 
				fight 
			WHERE 
				competition = OLD.competition
				AND style = OLD.style
				AND weight = OLD.weight
				AND cround IN (qualification_round_id, selection_round_id, final_round_id)
			ORDER BY 
				num
		);

		fcount       := icount(fid);
		start_offset := floor(wcount/2);
		fpos         := idx(fid, NEW.id);
		-- offset  for free fights
		y            := ceil(cast(fpos as double precision)/2);
		-- winner place for fight
		z            := floor(fpos/y);
		fight_id     := fid[start_offset+y];

		IF wcount%2 = 0 THEN
			IF z = 1 THEN 
				UPDATE fight SET red = NEW.winner WHERE id = fight_id;
			ELSE
				UPDATE fight SET blue = NEW.winner WHERE id = fight_id;
			END IF;
		ELSE
			IF z = 2 THEN 
				fight_id = fid[start_offset+y+1];
				UPDATE fight SET red = NEW.winner WHERE id = fight_id;
			ELSE
				UPDATE fight SET blue = NEW.winner WHERE id = fight_id;
			END IF;
		END IF;
	ELSIF 
		   NEW.cround = consolation_round_id_1 
		OR NEW.cround = consolation_round_id_2
	THEN
		fight_id := 0;
		SELECT 
			INTO fid 
			array( 
				SELECT 
					id 
				FROM 
					fight 
				WHERE 
					competition = OLD.competition
					AND style = OLD.style
					AND weight = OLD.weight
					AND cround = NEW.cround
				ORDER BY 
					num
			);
		
		fpos     := idx(fid, NEW.id);
		fight_id := fid[fpos+1];
		
		UPDATE fight SET red = NEW.winner WHERE id = fight_id;
	END IF;

	-- nothing to do if we not on half final tour, 'cause repechage based on half final fights results 
	IF NEW.ctour IS NULL OR NEW.ctour <> half_final_id
	THEN
		RETURN NEW;
	END IF;

	-- from now we try to create repechage fights
	-- check that fights allready rceated

	SELECT 
		INTO fid 
		array (
			SELECT 
				id 
			FROM 
				fight
			WHERE 
				ctour = half_final_id
				AND competition = NEW.competition
				AND style = NEW.style
				AND weight = NEW.weight
			ORDER BY 
				id
		);
		
	fpos := idx(fid, NEW.id);

	IF fpos = 1 THEN 
		cround_id = consolation_round_id_1;
	ELSIF fpos = 2 THEN
		cround_id = consolation_round_id_2;
	ELSE 
		cround_id = 0;
	END IF;

	IF cround_id = 0 THEN
		RAISE EXCEPTION 'ROUND ID CANNOT BE "0"';
	END IF;

	SELECT 
		INTO tmp 
		count(id) 
	FROM 
		fight
	WHERE 
		competition = NEW.competition 
		AND style = NEW.style 
		AND weight = NEW.weight
		AND cround = cround_id;

	IF tmp > 0 THEN RETURN NEW; END IF;

	-- take maximum fight num from that we begin
	SELECT 
		INTO fight_num 
		max(num) 
	FROM 
		fight 
	WHERE 
		competition = NEW.competition
		AND style = NEW.style
		AND weight = NEW.weight;

	tmp := 0;
			
	-- loop for fights on qualification and selection rounds
	FOR 
		fightrec 
	IN 
		SELECT 
			red, blue 
		FROM 
			fight 
		WHERE 
			winner = NEW.winner
			AND cround IN (qualification_round_id, selection_round_id)
			AND competition = NEW.competition
			AND style = NEW.style 
			AND weight = NEW.weight
		ORDER BY 
			id
	LOOP
		-- make desision on what place wrestler must take on free fight
		IF fightrec.red = NEW.winner THEN 
			wrestler_id := fightrec.blue;
		ELSE
			wrestler_id := fightrec.red;
		END IF;
			
		IF tmp > 1 THEN 
			-- create repechage fight with on free place
			fight_num := fight_num + 1;
			INSERT INTO fight (competition, style, weight, cround, num, mat, blue)
			VALUES (OLD.competition, OLD.style, OLD.weight, cround_id, fight_num, OLD.mat, wrestler_id);
		ELSE 
			-- find red adn blue wrestlers for first repechage fight
			IF tmp = 0 THEN 
				fight_red := wrestler_id; 
			ELSIF 
				tmp = 1 
			THEN 
				fight_blue := wrestler_id; 
			END IF;
				
			-- store wrestlers on first repechage fight with two iteration
			IF fight_red <> 0 AND fight_blue <> 0 THEN
				fight_num := fight_num + 1;
				INSERT INTO fight (competition, style, weight, cround, num, mat, red, blue)
				VALUES (OLD.competition, OLD.style, OLD.weight, cround_id, fight_num, OLD.mat, fight_red, fight_blue);
			END IF;
		END IF;

		tmp := tmp + 1;
	END LOOP;
	
	RETURN NEW;
END
$$ language plpgsql;
