-- fight_update_byteam

CREATE OR REPLACE FUNCTION fight_update_byteam() RETURNS TRIGGER AS $$
DECLARE
	winner_id 		integer;
	looser_id 		integer;
	
	winner_points   integer;
	looser_points 	integer;
	
	byteam 			boolean;
	fight_in_match 	integer;
	match_id  		integer;
	drec			record;

	looser_team  	integer;
	winner_team 	integer;
BEGIN
	winner_id := NEW.winner;
	match_id  := NEW.match;

	SELECT INTO byteam c.byteam FROM competition as c WHERE c.id = NEW.competition;
	IF byteam = FALSE THEN RETURN NEW; END IF;
	
	IF winner_id IS NULL THEN RETURN NEW; END IF;
	
	IF OLD.winner IS NOT NULL 
	THEN
		-- take old points
		IF OLD.winner = OLD.blue 
		THEN
			select into looser_team t.id from team as t, competition_wrestler as ct where ct.competition = OLD.competition AND ct.wrestler = OLD.red and t.id = ct.team;
			select into winner_team t.id from team as t, competition_wrestler as ct where ct.competition = OLD.competition AND ct.wrestler = OLD.blue and t.id = ct.team;
			looser_points := sum_intarray(OLD.red_score);
			winner_points := sum_intarray(OLD.blue_score);
		ELSE
			select into looser_team t.id from team as t, competition_wrestler as ct where ct.competition = OLD.competition and ct.wrestler = OLD.blue and t.id = ct.team;
			select into winner_team t.id from team as t, competition_wrestler as ct where ct.competition = OLD.competition and ct.wrestler = OLD.red and t.id = ct.team;
			looser_points := sum_intarray(OLD.blue_score);
			winner_points := sum_intarray(OLD.red_score);
		END IF;
			
		-- remove old points for winner
		UPDATE 
			competition_team
		SET 
			tech_points_win    = tech_points_win   - winner_points,
			tech_points_loose  = tech_points_loose - looser_points
		WHERE
			team        = winner_team AND
			competition = OLD.competition
		;

		-- remove old poins for looser
		UPDATE 
			competition_team
		SET 
			tech_points_win   = tech_points_win   - looser_points,
			tech_points_loose = tech_points_loose - winner_points
		WHERE
			team        = looser_team AND
			competition = OLD.competition
		;	
			
		UPDATE 
			match
		SET 
			team_a_win = team_a_win - CASE WHEN team_a = (SELECT team FROM competition_wrestler WHERE wrestler = winner_id AND competition = OLD.competition) THEN 1 ELSE 0 END,
			team_b_win = team_b_win - CASE WHEN team_b = (SELECT team FROM competition_wrestler WHERE wrestler = winner_id AND competition = OLD.competition) THEN 1 ELSE 0 END
		WHERE
			id = OLD.match;
	END IF;
		
	IF NEW.winner = NEW.blue 
	THEN
		select into looser_team t.id from team as t, competition_wrestler as ct where ct.competition = NEW.competition and ct.wrestler = NEW.red and t.id = ct.team;
		select into winner_team t.id from team as t, competition_wrestler as ct where ct.competition = NEW.competition and ct.wrestler = NEW.blue and t.id = ct.team;
		looser_points := sum_intarray(NEW.red_score);
		winner_points := sum_intarray(NEW.blue_score);
	ELSE
		select into looser_team t.id from team as t, competition_wrestler as ct where ct.competition = NEW.competition and ct.wrestler = NEW.blue and t.id = ct.team;
		select into winner_team t.id from team as t, competition_wrestler as ct where ct.competition = NEW.competition and ct.wrestler = NEW.red and t.id = ct.team;
		looser_points := sum_intarray(NEW.blue_score);
		winner_points := sum_intarray(NEW.red_score);
	END IF;

	UPDATE 
		competition_team
	SET 
		tech_points_win    = tech_points_win   + winner_points,
		tech_points_loose  = tech_points_loose + looser_points
	WHERE
		team        = winner_team AND
		competition = NEW.competition 
	;

	UPDATE 
		competition_team
	SET
		tech_points_win   = tech_points_win   + looser_points,
		tech_points_loose = tech_points_loose + winner_points
	WHERE
		team        = looser_team AND
		competition = NEW.competition 
	;	
		
	UPDATE 
		match
	SET 
		team_a_win = team_a_win + CASE WHEN team_a = (SELECT team FROM competition_wrestler WHERE wrestler = winner_id AND competition = NEW.competition) THEN 1 ELSE 0 END,
		team_b_win = team_b_win + CASE WHEN team_b = (SELECT team FROM competition_wrestler WHERE wrestler = winner_id AND competition = NEW.competition) THEN 1 ELSE 0 END
	WHERE
		id = NEW.match;

	RETURN NEW;
END
$$ language plpgsql;