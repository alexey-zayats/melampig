-- functions.sql

-- CREATE OR REPLACE TYPE sex AS ENUM ('male', 'female');
CREATE OR REPLACE FUNCTION random(numeric, numeric)
RETURNS numeric AS
$$
   SELECT ($1 + ($2 - $1) * random())::numeric;
$$ LANGUAGE 'sql' VOLATILE;

CREATE OR REPLACE FUNCTION t2p (t LTREE) RETURNS TEXT AS $$
BEGIN
  RETURN '/' || translate(t::TEXT, '.', '/');
END
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION t2pr (t LTREE) RETURNS TEXT AS $$
BEGIN
  RETURN translate(t::TEXT, '.', '/');
END
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION p2t (p TEXT) RETURNS LTREE AS $$
BEGIN
  RETURN translate(ltrim(normalpath(p), '/'), '/', '.')::LTREE;
END
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION match4every( num integer ) RETURNS INTEGER AS $$
BEGIN
	RETURN cast((num!)/(2*((num-2)!)) as int);
END
$$ LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION not_insert_update () RETURNS TRIGGER AS $$
BEGIN
	IF TG_OP = 'INSERT' THEN
		RAISE EXCEPTION 'INSERT into table % not allowed', TG_RELNAME;
	ELSIF TG_OP = 'UPDATE' THEN
		RAISE EXCEPTION 'UPDATE of table % not allowed', TG_RELNAME;
	END IF;
	RETURN NULL;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION obj_before_insert_update () RETURNS TRIGGER AS $$
BEGIN
	NEW.relname = TG_RELNAME;
	NEW.updated := now();
	IF TG_OP = 'UPDATE' THEN
		IF NEW.id IS NULL THEN new.id := old.id; END IF; 
	ELSIF TG_OP = 'INSERT' THEN
		IF NEW.id IS NULL THEN NEW.id := nextval('obj_id_seq'); END IF;
		NEW.guid := uuid_generate_v4();
		IF NEW.created IS NULL THEN NEW.created := now(); END IF;
		IF NEW.name IS NULL THEN
    		NEW.name := TG_RELNAME || CAST(NEW.id as text);
		END IF;
	END IF;
	RETURN NEW;
END;
$$ LANGUAGE plpgsql SECURITY DEFINER;

CREATE OR REPLACE FUNCTION tree_after_update () RETURNS TRIGGER AS $$
BEGIN
	IF NEW.trail <> OLD.trail THEN
		UPDATE tree SET trail = (NEW.trail || NEW.name) WHERE pid = NEW.id AND id <> NEW.id AND relname = TG_RELNAME;
	END IF;
	RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION update_title_onfio () RETURNS TRIGGER AS $$
BEGIN
	NEW.title := NEW.lastname || ' ' || NEW.firstname;
	RETURN NEW;
END;
$$ LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION update_match_title () RETURNS TRIGGER AS $$
BEGIN
	NEW.title := (SELECT title FROM team WHERE id = NEW.team_a) || ' vs ' || (Select title FROM team WHERE id = NEW.team_b);
	RETURN NEW;
END;
$$ LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION tree_after_delete () RETURNS TRIGGER AS $$
BEGIN
	DELETE FROM tree WHERE trail <@ OLD.trail;
	RETURN OLD;
END;
$$ LANGUAGE plpgsql;


-- Trigger function for INSERT or UPDATE in TREE (BEFORE INSERT/UPDATE)
CREATE OR REPLACE FUNCTION tree_before_insert_update () RETURNS TRIGGER AS $$
BEGIN
  IF NEW.pid = NEW.id OR NEW.pid = 0 THEN NEW.pid := NULL; END IF;
  IF NEW.trail IS NOT NULL AND NEW.pid IS NULL THEN
      SELECT INTO NEW.pid id FROM tree WHERE relname = TG_RELNAME AND trail @> NEW.trail ORDER BY trail DESC LIMIT 1;
      IF NEW.pid = NEW.id THEN NEW.pid := NULL; END IF;
  END IF;
  IF NEW.pid IS NULL THEN
      IF EXISTS (SELECT * FROM tree WHERE relname = TG_RELNAME AND pid IS NULL AND name = NEW.name AND id <> NEW.id) THEN
        RAISE EXCEPTION 'Root % with name "%" already exists', TG_RELNAME, NEW.name;
      END IF;
      IF NEW.trail IS NULL THEN
      	NEW.trail := text2ltree(NEW.name);
      END IF;
  ELSE
 --	IF NEW.trail IS NULL THEN
	    SELECT INTO NEW.trail trail FROM tree WHERE id = NEW.pid AND relname = TG_RELNAME;
	    IF NOT FOUND THEN
	      RAISE EXCEPTION 'Cannot find % % (for UP field)', TG_RELNAME, NEW.pid;
	    END IF;
	    NEW.trail = NEW.trail || NEW.name;
--	END IF;
    IF TG_OP = 'UPDATE' THEN
      IF NEW.pid <> OLD.pid THEN
        IF OLD.trail @> NEW.trail THEN
          RAISE EXCEPTION 'Cannot move % % into his subtree; % %', TG_RELNAME, NEW.id, OLD.trail, NEW.trail;
        END IF;
      END IF;
    END IF;
  END IF;
  NEW.level = nlevel(NEW.trail);
  RETURN NEW;
END;
$$ LANGUAGE plpgsql SECURITY DEFINER;

CREATE OR REPLACE FUNCTION link_before_insert_update () RETURNS TRIGGER AS $$
DECLARE
	src integer;
	dst integer;
BEGIN
	src := 0;
	dst := 0;
	SELECT INTO src id FROM obj WHERE id = NEW.src;
	IF NOT FOUND THEN 
		RAISE EXCEPTION 'Cannot find src(%) for %', NEW.src, TG_RELNAME;
	END IF;

	SELECT INTO dst id FROM obj WHERE id = NEW.dst;
	IF NOT FOUND THEN 
		RAISE EXCEPTION 'Cannot find dst(%) for %', NEW.dst, TG_RELNAME;
	END IF;
	RETURN NEW;
END;
$$ LANGUAGE plpgsql SECURITY DEFINER;


CREATE OR REPLACE FUNCTION translit (s text) RETURNS text AS $$
DECLARE
	chars text[];
	i integer;
	result text;
	map hstore := '
1072=>a, 1040=>A,
1073=>b, 1041=>B,
1074=>v, 1042=>V,
1075=>g, 1043=>G,
1076=>d, 1044=>D,
1077=>e, 1045=>E,
1105=>jo, 1025=>Jo,
1078=>zh, 1046=>Zh,
1079=>z, 1047=>Z,
1080=>i, 1048=>I,
1081=>j, 1049=>J,
1082=>k, 1050=>K,
1083=>l, 1051=>L,
1084=>m, 1052=>M,
1085=>n, 1053=>N,
1086=>o, 1054=>O,
1087=>p, 1055=>P,
1088=>r, 1056=>R,
1089=>s, 1057=>S,
1090=>t, 1058=>T,
1091=>u, 1059=>U,
1092=>f, 1060=>F,
1093=>h, 1061=>H,
1094=>c, 1062=>C,
1095=>ch, 1063=>Ch,
1096=>sh, 1064=>Sh,
1097=>sch,1065=>Sch,
1099=>y, 1067=>Y,
1101=>je, 1069=>Je,
1102=>ju, 1070=>Ju,
1103=>ja, 1071=>Ja,
1069=>Je
';
	akey text;
	code integer;
BEGIN
	result := '';
	chars := regexp_split_to_array(s, '');
	FOR i IN 1..array_upper(chars,1) LOOP
	    code = ascii(chars[i]);
		akey = cast(code as text);
		IF map ? akey THEN
			result := result || (map->akey);
		END IF;
		IF code > 47 AND code < 58 THEN result := result || chars[i]; END IF;
		IF code > 64 AND code < 91 THEN result := result || chars[i]; END IF;
		IF code > 96 AND code < 123 THEN result := result || chars[i]; END IF;
	END LOOP;
	return result;
END
$$ language plpgsql;

CREATE OR REPLACE FUNCTION sum_intarray(inarray int[]) RETURNS INTEGER AS $$
DECLARE
	i integer;
	result integer;
BEGIN
	result := 0;
	FOR i in 1..#inarray 
	LOOP
		result := result + inarray[i];
	END LOOP;

	RETURN result;
END
$$ language plpgsql;

