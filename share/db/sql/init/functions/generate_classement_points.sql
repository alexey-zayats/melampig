-- generate_classement_points

CREATE OR REPLACE FUNCTION generate_classement_points(cid integer, wid integer, sid integer) RETURNS INTEGER AS $$
DECLARE
	frec RECORD;
	winner_id INTEGER;
	looser_id INTEGER;
	win_points INTEGER;
	looser_points INTEGER;
BEGIN

	-- clear wreslters points for competition on style and weight
	UPDATE 
		competition_wrestler
	SET
		classify_points = 0,
		tech_points_win = 0,
		tech_points_loose = 0,
		vt = 0,
		va = 0,
		vb = 0,
		vf = 0,
		ev = 0,
		ex = 0,
		st = 0,
		sp = 0,
		pp = 0,
		po = 0,
		e2 = 0
	WHERE
		competition = cid AND
		weight = wid AND
		style = sid;
	
	FOR frec IN
		SELECT 
			f.id, f.num, f.red, f.red_score, f.blue, f.blue_score, f.winner, f.classify, c.name as cname, c.win, c.fail, cw1.sorder as rn, cw2.sorder as bn
		FROM
			fight as f 
				left join classify as c ON c.id = f.classify
				left join competition_wrestler as cw1 ON cw1.wrestler = f.red
				left join competition_wrestler as cw2 ON cw2.wrestler = f.blue
		WHERE
			f.competition = cid AND
			f.weight = wid AND
			f.style = sid
		ORDER BY
			f.id
	LOOP
		IF frec.win IS NULL THEN frec.win := 0; END IF;
		IF frec.fail IS NULL THEN frec.fail := 0; END IF;

		IF frec.blue = frec.winner THEN
			winner_id     := frec.blue;
			win_points    := sum_intarray(frec.blue_score);
			looser_id     := frec.red;
			looser_points := sum_intarray(frec.red_score);
		ELSE
			winner_id     := frec.red;
			win_points    := sum_intarray(frec.red_score);
			looser_id     := frec.blue;
			looser_points := sum_intarray(frec.blue_score);
		END IF;

		-- store points for fight winner 
		UPDATE 
			competition_wrestler 
		SET 
			classify_points   = classify_points   + frec.win,
			tech_points_win   = tech_points_win   + win_points,
			tech_points_loose = tech_points_loose + looser_points,
			vt = vt + case when frec.cname = 'vt' then 1 else 0 end,
			va = va + case when frec.cname = 'va' then 1 else 0 end,
			vb = vb + case when frec.cname = 'vb' then 1 else 0 end,
			vf = vf + case when frec.cname = 'vf' then 1 else 0 end,
			ev = ev + case when frec.cname = 'ev' then 1 else 0 end,
			ex = ex + case when frec.cname = 'ex' then 1 else 0 end,
			st = st + case when frec.cname = 'st' then 1 else 0 end,
			sp = sp + case when frec.cname = 'sp' then 1 else 0 end,
			pp = pp + case when frec.cname = 'pp' then 1 else 0 end,
			po = po + case when frec.cname = 'po' then 1 else 0 end,
			e2 = e2 + case when frec.cname = 'e2' then 1 else 0 end
		WHERE
			wrestler = winner_id AND
			competition = cid AND 
			style = sid AND
			weight = wid;

		-- store points for fight looser
		UPDATE 
			competition_wrestler 
		SET 
			classify_points   = classify_points   + frec.fail,
			tech_points_win   = tech_points_win   + looser_points,
			tech_points_loose = tech_points_loose + win_points
		FROM
			classify as c
		WHERE
			wrestler = looser_id AND
			competition = cid AND 
			style = sid AND
			weight = wid;			
	END LOOP;
	
	RETURN 1;
END
$$ language plpgsql;


