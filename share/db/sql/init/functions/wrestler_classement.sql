-- wrestler_classement

CREATE OR REPLACE FUNCTION wrestler_classement(cid integer, wid integer, sid integer) RETURNS INT[] AS $$
DECLARE
	final_id integer;
	consolation_id int[];
	first_id integer;
	second_id integer;
	third_id integer;
	fifth_id integer;
	wrestlers integer[];
	teams hstore;
	team record;
	tmp integer;
	is_byteam boolean;
BEGIN	
	wrestlers := '{}'::int[];
	teams := ''::hstore;

	SELECT INTO final_id id FROM cround WHERE name = 'final';
	SELECT INTO consolation_id array(select id FROM cround WHERE name IN ('consolation-1', 'consolation-2'));
	SELECT INTO is_byteam byteam FROM competition WHERE id = cid;

	-- find 1-st and 2-nd placed wrestler;
	SELECT INTO first_id, second_id 
		winner as first_id, case when red = winner then blue else red end as second_id 
	FROM
		fight 
	WHERE 
		competition = cid AND 
		weight = wid AND 
		style = sid AND 
		cround = final_id;

	IF is_byteam THEN
		teams := teams || cast(cast(first_id as text) || '=>1' as hstore);
		teams := teams || cast(cast(second_id as text) || '=>1' as hstore);
	ELSE
		wrestlers := wrestlers + first_id;
		wrestlers := wrestlers + second_id;
	END IF;
	
	-- find 3-d placed wrestler;
	FOR tmp IN 1..(#consolation_id)
	LOOP 
		SELECT  INTO third_id
			winner
		FROM
			fight 
		WHERE 
			competition = cid AND 
			weight = wid AND 
			style = sid AND 
			cround = consolation_id[tmp]
		ORDER BY
			id DESC
		LIMIT 1;

		IF FOUND THEN
			IF is_byteam THEN
				teams := teams || cast(cast(third_id as text) || '=>1' as hstore);
			ELSE
				wrestlers := wrestlers + third_id;
			END IF;
		END IF;
	END LOOP;
	
	-- find 5-th placed wrestler;
	FOR tmp IN 1..(#consolation_id)
	LOOP 
		SELECT  INTO fifth_id
			case when winner = red then blue else red end
		FROM
			fight 
		WHERE 
			competition = cid AND 
			weight = wid AND 
			style = sid AND 
			cround = consolation_id[tmp]
		ORDER BY
			id DESC
		LIMIT 1;
		
		IF FOUND THEN
			wrestlers := wrestlers + fifth_id;
		END IF;

	END LOOP;

	tmp := generate_classement_points(cid, wid, sid);

	-- other wrestlers classemenet
	-- case when is_byteam then c.team else c.wrestler end 
	FOR tmp IN 
		SELECT 
			c.wrestler
		FROM 
			competition_wrestler as c 
				left join wrestler as w on w.id = c.wrestler 
		WHERE	
			c.competition = cid AND 
			c.weight = wid AND 
			c.style = sid AND
			not intset(c.wrestler) && wrestlers
		ORDER BY
			classify_points desc,
			tech_points_win desc,
			vt desc, 
			va desc,
			vb desc,
			vf desc,
			ev desc,
			ex desc,
			st desc, 
			sp desc, 
			pp desc,
			po desc,
			e2 desc,
			tech_points_loose asc,
			w.title asc
	LOOP
		wrestlers := wrestlers + tmp;
	END LOOP;
			
	RETURN wrestlers;
		
END
$$ language plpgsql;
