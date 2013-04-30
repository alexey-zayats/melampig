UPDATE 
    fight
SET 
    red_score[1] = cast(random()*10 as int),
    red_score[2] = cast(random()*10 as int),
    red_score[3] = cast(random()*10 as int),
    blue_score[1] = cast(random()*10 as int), 
    blue_score[2] = cast(random()*10 as int), 
    blue_score[3] = cast(random()*10 as int),
    winner = case cast(random()*10 as int)%2 when 1 then blue else red end,
    classify = (select id from classify order by random() limit 1)
WHERE
    competition = 478
    AND (winner IS NULL AND classify IS NULL)
    AND (red IS NOT NULL AND blue IS NOT NULL)
;