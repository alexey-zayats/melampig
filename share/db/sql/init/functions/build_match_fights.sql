-- build_match_fights

CREATE OR REPLACE FUNCTION build_match_fights(match_id integer) RETURNS integer AS $$
DECLARE
	matches record;
	weight_id integer;
	red_id integer;
	blue_id integer;
	fight_num integer;
BEGIN
	SELECT INTO matches * from match where id = match_id;

	fight_num := 0;
	SELECT INTO fight_num count(num) from fight where competition = matches.competition AND style = matches.style;

	FOR weight_id IN SELECT w.id FROM competition_weight as cw, weight as w WHERE cw.competition = matches.competition and w.id = cw.weight ORDER BY w.title
	LOOP
		SELECT INTO red_id  cw.wrestler FROM competition_wrestler as cw WHERE cw.competition = matches.competition AND cw.team = matches.team_a AND cw.style = matches.style AND cw.weight = weight_id LIMIT 1; 
		SELECT INTO blue_id cw.wrestler FROM competition_wrestler as cw WHERE cw.competition = matches.competition AND cw.team = matches.team_b AND cw.style = matches.style AND cw.weight = weight_id LIMIT 1;
		fight_num := fight_num + 1;
		INSERT INTO fight (competition, style, weight, match, circle, cround, ctour, num, mat, red, blue)
		        VALUES (matches.competition, matches.style, weight_id, matches.id, matches.circle, matches.cround, matches.ctour, fight_num, matches.mat, red_id, blue_id);
	END LOOP;
	RETURN fight_num;
END 
$$ language plpgsql;
