

CREATE OR REPLACE FUNCTION remove_competition() RETURNS TRIGGER AS $$ 
DECLARE
	cid INTEGER;
	arow RECORD;
BEGIN
	cid = OLD.id; 

	DELETE FROM fight_log USING fight as f WHERE f.competition = cid AND fight_log.fight = f.id;
	DELETE FROM fight WHERE competition = cid;
	DELETE FROM match WHERE competition = cid;
	
	FOR arow IN SELECT wrestler FROM competition_wrestler WHERE competition = cid LOOP
		DELETE FROM competition_wrestler WHERE competition = cid AND wrestler = arow.wrestler;
	END LOOP;
	
	FOR arow IN SELECT team FROM competition_team WHERE competition = cid LOOP
		DELETE FROM competition_team WHERE competition = cid AND team = arow.team;
	END LOOP;

	DELETE FROM competition_wrestler WHERE competition = cid ;
	DELETE FROM competition_weight WHERE competition = cid;
	DELETE FROM competition_style WHERE competition = cid;
	DELETE FROM competition_team WHERE competition = cid;
	DELETE FROM competition_referee WHERE competition = cid;
	RETURN OLD; 
END
$$ language plpgsql;

CREATE OR REPLACE FUNCTION remove_fight() RETURNS TRIGGER AS $$ 
DECLARE
BEGIN
	DELETE FROM fight_log WHERE fight = OLD.id;	
	RETURN OLD; 
END
$$ language plpgsql;

CREATE OR REPLACE FUNCTION remove_match() RETURNS TRIGGER AS $$ 
DECLARE
BEGIN
	DELETE FROM fight WHERE match = OLD.id;
	RETURN OLD; 
END
$$ language plpgsql;


