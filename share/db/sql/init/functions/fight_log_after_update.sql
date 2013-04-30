-- fight_log_after_update

CREATE OR REPLACE FUNCTION fight_log_after_update() RETURNS TRIGGER AS $$
DECLARE
	action_name  varchar;
	is_blue      boolean;
	round_name   varchar;
	overtime_round integer; 
	round_pos integer; 
BEGIN
	overtime_round := 0;
	round_pos := 0;
	
	SELECT INTO action_name name FROM action WHERE id = NEW.action;
    
	SELECT INTO round_name name FROM fround WHERE id = NEW.fround;
    
	SELECT INTO is_blue CASE WHEN blue = NEW.wrestler THEN true ELSE false END FROM fight WHERE id = NEW.fight;
    
	IF round_name = 'first.overtime'  THEN overtime_round := 1; END IF;
	IF round_name = 'second.overtime' THEN overtime_round := 2; END IF;
	IF round_name = 'third.overtime'  THEN overtime_round := 3; END IF;

	IF round_name = 'first'  THEN round_pos := 1; END IF;
	IF round_name = 'second' THEN round_pos := 2; END IF;
	IF round_name = 'third'  THEN round_pos := 3; END IF;
	
	IF action_name = 'score' THEN
		IF round_pos > 0 AND round_pos < 4 THEN
			IF is_blue THEN
				UPDATE fight SET blue_score[round_pos] = blue_score[round_pos] + NEW.score WHERE id = NEW.fight;
			ELSE 
				UPDATE fight SET red_score[round_pos] = red_score[round_pos] + NEW.score WHERE id = NEW.fight;
			END IF;
		END IF;
	
		IF overtime_round > 0 AND overtime_round < 4 THEN
			IF is_blue THEN
				UPDATE fight SET blue_score[overtime_round] = blue_score[overtime_round] + NEW.score WHERE id = NEW.fight;
			ELSE 
				UPDATE fight SET red_score[overtime_round] = red_score[overtime_round] + NEW.score WHERE id = NEW.fight;
			END IF;
		END IF;
	END IF;

	IF action_name = 'pen' THEN
		IF round_pos > 0 AND round_pos < 4 THEN
			IF is_blue THEN
				UPDATE fight SET blue_pen[round_pos] = blue_pen[round_pos] + NEW.score WHERE id = NEW.fight;
			ELSE 
				UPDATE fight SET red_pen[round_pos] = red_pen[round_pos] + NEW.score WHERE id = NEW.fight;
			END IF;
		END IF;

		IF overtime_round > 0 AND overtime_round < 4 THEN
			IF is_blue THEN
				UPDATE fight SET blue_pen[overtime_round] = blue_pen[overtime_round] + NEW.score WHERE id = NEW.fight;
			ELSE 
				UPDATE fight SET red_pen[overtime_round] = red_pen[overtime_round] + NEW.score WHERE id = NEW.fight;
			END IF;
		END IF;
	END IF;

	IF action_name = 'pass' THEN
		IF round_pos > 0 AND round_pos < 4 THEN
			IF is_blue THEN
				UPDATE fight SET blue_pass[round_pos] = blue_pass[round_pos] + NEW.score WHERE id = NEW.fight;
			ELSE 
				UPDATE fight SET red_pass[round_pos] = red_pass[round_pos] + NEW.score WHERE id = NEW.fight;
			END IF;
		END IF;

		IF overtime_round > 0 AND overtime_round < 4 THEN
			IF is_blue THEN
				UPDATE fight SET blue_pass[overtime_round] = blue_pass[overtime_round] + NEW.score WHERE id = NEW.fight;
			ELSE 
				UPDATE fight SET red_pass[overtime_round] = red_pass[overtime_round] + NEW.score WHERE id = NEW.fight;
			END IF;
		END IF;
	END IF;

	IF action_name = 'start' THEN
		UPDATE fight SET fstart = NEW.etime WHERE id = NEW.fight;
	END IF;

	IF action_name = 'stop' THEN
		UPDATE fight SET fstop = NEW.etime WHERE id = NEW.fight;
	END IF;

	RETURN NEW;
END
$$ language plpgsql;
