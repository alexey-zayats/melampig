create view 
	v_fight_video 
	as
	select
		f.id "Id", 
		f.num "Number", 
		c.byteam AS "IsTeam",
		f.competition "TournamentId", 
		c.title "TournamentName", 
		w.title "Weight", 
		mat.name "Mat",
		style.name AS "StyleCode", 
		style.title AS "StyleName", 
		case
		  when c.byteam then circle.displayname  --"Circle"
		  when ct.Num = 1 then 'FINAL'
		  when cr.name = 'consolation-1' and ct.title <> 'Финал-III' then 'FINAL III'
		  when cr.name = 'consolation-2' and ct.title <> 'Финал-III'  then 'FINAL III'
		  when ct.title = 'Финал-III' then 'FINAL 3-5'
		  else '1/' || cast(ct.num as character varying) || ' FINALE'
		end "FightTitle", 		
		r.lastname "RedLastName", 
		r.firstname "RedFirstName",
		case when length(tr.title) > length(gr.title)  then gr.title else tr.title end "RedTeamName",
		gr.alpha3 "RedFlagCode", 
		b.lastname "BlueLastName", 
		b.firstname "BlueFirstName", 
		case when length(tb.title) > length(gb.title)  then gb.title else tb.title end "BlueTeamName", 
		gb.alpha3 "BlueFlagCode",
		f.winner AS "ConfirmedWinner"
	from 
		fight f
		inner join competition c on c.id = f.competition
		inner join wrestler r on r.id = f.red
		inner join wrestler b on b.id = f.blue
		inner join weight w on w.id = f.weight
		inner join public.mat mat ON mat.id = f.mat
		inner join style style ON f.style = style.id 
		left outer join circle on circle.id = f.circle
		left outer join cround cr ON cr.id = f.cround 
		left outer join ctour ct ON ct.id = f.ctour 
		inner join competition_wrestler cwr on cwr.competition = c.id and cwr.wrestler = f.red and cwr.style = f.style
		inner join wrestler wr on wr.id = cwr.wrestler
		inner join team tr on tr.id = cwr.team
		left outer join geo gr on gr.id = tr.geo
		inner join competition_wrestler cwb on cwb.competition = c.id and cwb.wrestler = f.blue and cwb.style = f.style
		inner join wrestler wb on wb.id = cwb.wrestler
		inner join team tb on tb.id = cwb.team
		left outer join geo gb on gb.id = tb.geo;