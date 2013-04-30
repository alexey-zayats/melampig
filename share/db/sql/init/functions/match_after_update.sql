-- match_after_update

CREATE OR REPLACE FUNCTION match_before_update() RETURNS TRIGGER AS $$
DECLARE
	winner_id integer;
	looser_id integer;
	looser_points integer;
	win_points integer;
	team_id integer;
	team_a integer;
	team_b integer;
	match_id integer;
	x integer;
	circles int[];
BEGIN

	team_a := 0;
	team_b := 0;
	x := 0;
	
	IF NEW.winner IS NULL THEN
	
		
		SELECT INTO team_id count(id) from fight where match = NEW.id;
		x := NEW.team_a_win + NEW.team_b_win;
		
		IF x < team_id THEN RETURN NEW; END IF;
		
		SELECT INTO NEW.classify c.id FROM classify as c WHERE c.name = 'team';
		IF NEW.team_a_win > NEW.team_b_win THEN
			NEW.winner = NEW.team_a;
		ELSE
			NEW.winner = NEW.team_b;
		END IF;
		
		NEW.classify := (select c.id FROM classify as c WHERE c.name = 'team');
	END IF;

	-- clear old values
	IF OLD.winner IS NOT NULL
	THEN
		winner_id := OLD.winner;

		-- take old points
		IF OLD.winner = OLD.team_a 
		THEN
			looser_id     := OLD.team_b;
			looser_points := OLD.team_b_win;
			win_points    := OLD.team_a_win;
		ELSE
			looser_id     := OLD.team_a;
			looser_points := OLD.team_a_win;
			win_points    := OLD.team_b_win;
		END IF;
		
		-- remove old points for winner
		UPDATE 
			competition_team 
		SET 
			classify_points    = classify_points   - c.win,
			fight_win    = fight_win   - win_points,
			fight_loose  = fight_loose - looser_points
		FROM
			classify as c
		WHERE
			team    = winner_id AND
			competition = OLD.competition AND 
			style       = OLD.style AND
			c.id        = OLD.classify;

		-- remove old poins for looser
		UPDATE 
			competition_team 
		SET 
			classify_points    = classify_points   - c.fail
			,fight_win  = fight_win  - looser_points
			,fight_loose = fight_loose - win_points
		FROM
			classify as c
		WHERE
			team    = looser_id AND
			competition = OLD.competition AND 
			style       = OLD.style AND
			c.id        = OLD.classify;	
	END IF;
	
	IF NEW.winner IS NULL THEN RETURN NEW; END IF;
	
	winner_id := NEW.winner;

	-- take looser and tech points for win and loose
	IF NEW.winner = NEW.team_a 
	THEN
		looser_id    := NEW.team_b;
		looser_points := NEW.team_b_win;
		win_points   := NEW.team_a_win;
	ELSE
		looser_id    := NEW.team_a;
		looser_points := NEW.team_a_win;
		win_points   := NEW.team_b_win;
	END IF;


	-- store points for fight winner 
	UPDATE 
		competition_team 
	SET 
		classify_points   = classify_points   + c.win,
		fight_win         = fight_win   + win_points,
		fight_loose       = fight_loose + looser_points
	FROM
		classify as c
	WHERE
		team = winner_id AND
		competition = NEW.competition AND 
		style = NEW.style AND
		c.id = NEW.classify;

	-- store points for fight looser
	UPDATE 
		competition_team 
	SET 
		classify_points = classify_points + c.fail,
		fight_loose = fight_loose + win_points,
		fight_win = fight_win + looser_points
	FROM
		classify as c
	WHERE
		team = looser_id AND
		competition = NEW.competition AND 
		style = NEW.style AND
		c.id = NEW.classify;

	raise notice 'classify: %', NEW.classify;
	return new;
END 
$$ LANGUAGE plpgsql; 
	
CREATE OR REPLACE FUNCTION match_after_update() RETURNS TRIGGER AS $$
DECLARE
	winner_id integer;
	looser_id integer;
	looser_points integer;
	win_points integer;
	team_id integer;
	team_a integer;
	team_b integer;
	match_id integer;
	x integer;
	y integer;
	circles int[];
	teams integer[][];
	grp_id integer;
BEGIN
	
	select into circles array(select id from circle where name like 'final%' order by name);

	SELECT INTO team_id id FROM match WHERE competition = NEW.competition AND style = NEW.style AND winner is NULL; 
	IF NOT FOUND THEN
		
		SELECT INTO team_id id FROM match WHERE competition = NEW.competition AND style = NEW.style AND cround = (select id from cround where name = 'final');
		IF FOUND THEN RETURN NEW; END IF;
		
		team_a := 0;
		team_b := 0;

		teams := '{{0,0,0,0,0},{0,0,0,0,0}}';
		
		x := 1;
		
		FOR grp_id IN 
			SELECT 
				cgroup
			FROM
				competition_team
			WHERE
				competition = NEW.competition AND 
				style = NEW.style 
			group by cgroup
		LOOP
			y := 1;
			FOR team_id IN 
				SELECT 
					team
				FROM 
					competition_team 
				WHERE 
					competition = NEW.competition AND 
					style = NEW.style AND
					cgroup = grp_id
				ORDER BY
					classify_points desc,
					fight_win desc,
					fight_loose asc,
					tech_points_win desc,
					tech_points_loose asc
			LOOP
				teams[x][y] = team_id;
				y := y + 1;
			END LOOP;
			x := x + 1;
		END LOOP;
		
		-- teams: {{1673,1679,1681,0,0,0},{1675,1677,1671,0,0,0}}
		
		x := 1;
		FOR y IN 1..5 LOOP
			
			if teams[1][y] = 0 then
				exit;
			end if;
			
			INSERT INTO match (competition, style, cround, circle, team_a, team_b) 
				VALUES (NEW.competition, NEW.style, (SELECT id FROM cround WHERE name = case when x = 1 then 'final' else 'consolation-1' end ), circles[x], teams[1][y], teams[2][y]) 
					RETURNING id INTO match_id;
				perform build_match_fights(match_id);
				
			x := x + 1;
		END LOOP;

		
	END IF;
	return NEW;
END 
$$ LANGUAGE plpgsql;
