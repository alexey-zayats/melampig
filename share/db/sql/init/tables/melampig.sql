-- --> Справочники --> ----

CREATE TABLE geo (
	title varchar,
	num varchar,
	alpha2 varchar,
	alpha3 varchar,
	flag bytea
) inherits (tree);

CREATE TRIGGER _1_obj_insert_update  BEFORE INSERT OR UPDATE ON geo FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();
CREATE TRIGGER _2_tree_insert_update BEFORE INSERT OR UPDATE ON geo FOR EACH ROW EXECUTE PROCEDURE tree_before_insert_update ();
CREATE TRIGGER _3_tree_after_update  AFTER  UPDATE           ON geo FOR EACH ROW EXECUTE PROCEDURE tree_after_update ();
CREATE TRIGGER _4_tree_after_delete  AFTER  DELETE           ON geo FOR EACH ROW EXECUTE PROCEDURE tree_after_delete();


CREATE TABLE gender (
	title varchar
) inherits (obj);

CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON gender FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();

-- Стили борьбы
CREATE TABLE style (
	title varchar,
	rnum integer,   -- round numbers
	rtime integer,  -- round time
	otime integer,  -- overtime
	rstime integer, -- rest time
	rtdir boolean,  -- round time direction
	otdir boolean,  -- overtime direction
	rstdir boolean  -- rest time direction
) inherits (obj);

CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON style FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();

-- Возрастные категории
CREATE TABLE age (
	title varchar
) inherits (obj);

CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON age FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();

-- Весовые категории
CREATE TABLE weight (
	title integer
) inherits (obj);

CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON weight FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();

-- Спортивные звания
CREATE TABLE rang (
	title varchar,
	acr varchar
) inherits (tree);

CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE  ON rang FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();
CREATE TRIGGER _2_tree_insert_update BEFORE INSERT OR UPDATE ON rang FOR EACH ROW EXECUTE PROCEDURE tree_before_insert_update ();
CREATE TRIGGER _3_tree_after_update  AFTER  UPDATE           ON rang FOR EACH ROW EXECUTE PROCEDURE tree_after_update ();
CREATE TRIGGER _4_tree_after_delete  AFTER  DELETE           ON rang FOR EACH ROW EXECUTE PROCEDURE tree_after_delete();

-- Должности судей
CREATE TABLE position (
	title varchar
) INHERITS (obj);

CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON position FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();


-- Раунды в соревнованиях
CREATE TABLE cround (
	title varchar
) inherits (obj);

CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON cround FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();


-- Туры в соревнованиях
CREATE TABLE ctour (
	title varchar,
	num integer
) inherits (obj);

CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON ctour FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();


-- Раунды в схватке
CREATE TABLE fround (
	title varchar
) inherits (obj);

CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON fround FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();

-- Типы соревнований
CREATE TABLE comptype (
	title varchar
) inherits (obj);

CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON comptype FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();


-- Действия в схватке
CREATE TABLE action (
	title varchar
) inherits (obj);

CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON action FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();


-- маты
CREATE TABLE mat (
	title varchar
) inherits (obj);

CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON mat FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();

-- классификационные очки
CREATE TABLE classify (
	win integer,
	fail integer,
	title varchar
) INHERITS (obj);

CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON classify FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();

-- роли участников соревнований 
CREATE TABLE role (
	title varchar
) inherits (obj);

CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON role FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();

-- круги соревнований при встречах каждый-с-каждым
CREATE TABLE circle (
	title varchar,
	displayname varchar
) inherits (obj);

CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON circle FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();

-- подгруппа
CREATE TABLE cgroup (
	title varchar
) inherits (obj);

CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON cgroup FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();

-- <-- Справочники <-------------------------------------------------------

-- контакт
CREATE TABLE contact (
	firstname varchar,
	lastname varchar,
	patronymic varchar,
	birthdate date,
	gender integer, -- fk on gender
	phone varchar,
	email varchar,
	picture bytea
) INHERITS (obj);

CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON contact FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();

-- типы адресов
CREATE TABLE addresstype (
	title varchar
) inherits (obj);

CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON addresstype FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();

-- адрес
CREATE TABLE address (
	contact integer,     -- fk on contact
	addresstype integer, -- fk on addresstype
	geo integer,         -- fk on geo
	postcode varchar,
	street varchar,
	house varchar
) inherits (obj);

CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON address FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();


-- Тренера
CREATE TABLE coach (
	title varchar,
	rang integer, -- fk on rang
	team integer  -- fk on team
) inherits (contact);

CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON coach FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();
CREATE TRIGGER _2_obj_insert_update BEFORE INSERT OR UPDATE ON coach FOR EACH ROW EXECUTE PROCEDURE update_title_onfio ();

CREATE TABLE coach_address (
) inherits (address);

CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON coach_address FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();

-- Борцы
CREATE TABLE wrestler (
	title varchar,
	rang  integer, -- fk on rang
	coach integer,  -- fk on coach
	geo integer     -- fk on geo
) inherits (contact);

CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON wrestler FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();
CREATE TRIGGER _2_obj_insert_update BEFORE INSERT OR UPDATE ON wrestler FOR EACH ROW EXECUTE PROCEDURE update_title_onfio ();

CREATE TABLE wrestler_address (
) inherits (address);
CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON wrestler_address FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();

-- Судьи
CREATE TABLE referee (
	title varchar,
	license varchar,
	rang  integer, -- fk on rang
	geo integer -- fk on geo
) inherits (contact);

CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON referee FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();
CREATE TRIGGER _2_obj_insert_update BEFORE INSERT OR UPDATE ON referee FOR EACH ROW EXECUTE PROCEDURE update_title_onfio ();

CREATE TABLE referee_address (
) inherits (address);
CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON referee_address FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();


-- Участник соревнований
CREATE TABLE member (
	title varchar
) inherits (contact);

CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON member FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();
CREATE TRIGGER _2_obj_insert_update BEFORE INSERT OR UPDATE ON member FOR EACH ROW EXECUTE PROCEDURE update_title_onfio ();

CREATE TABLE member_address (
) inherits (address);
CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON member_address FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();

-- Команды
CREATE TABLE team (
	title varchar,
	style integer,
	geo   integer
) inherits (obj);

CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON team FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();

--------------------------

-- Соревнования
CREATE TABLE competition (
	title  varchar,
	start  date,
	stop   date,
	age    integer,    -- fk on age
	comptype  integer, -- fk on comptype
	byteam boolean,
	geo    integer
) inherits (obj);

CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON competition FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();
CREATE TRIGGER _10_remove_competition BEFORE DELETE ON competition FOR EACH ROW EXECUTE PROCEDURE remove_competition ();

CREATE TABLE competition_style (
	competition integer, -- fk on competition
	style integer        -- fk on style
) inherits (obj);

CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON competition_style FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();

CREATE TABLE competition_weight (
	competition integer, -- fk on competition
	weight integer       -- fk on weight
) inherits (obj);

CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON competition_weight FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();

CREATE TABLE competition_team (
	competition integer, -- fk on competition
	team integer,        -- fk on team
	cgroup integer,      -- fk cgroup
	style integer,       -- fk on style
	sorder integer,
	classify_points integer default 0,
	fight_win 		integer default 0,
	fight_loose 	integer default 0,
	tech_points_win integer default 0,
	tech_points_loose integer default 0,
	rank 			integer default 0
) inherits (obj);

CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON competition_team FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();

CREATE TABLE competition_wrestler (
	competition integer, -- fk on competition
	team integer,        -- fk on team
	wrestler integer,    -- fk as wrestler
	style integer,       -- fk on style
	weight integer,      -- fk on weight
	heft real,
	classify_points integer default 0,
	tech_points_win integer default 0,
	tech_points_loose integer default 0,
	vt integer default 0,
	va integer default 0,
	vb integer default 0,
	vf integer default 0,
	ev integer default 0,
	ex integer default 0,
	st integer default 0,
	sp integer default 0,
	pp integer default 0,
	po integer default 0,
	e2 integer default 0,
	sorder integer
) inherits (obj);

CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON competition_wrestler FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();

CREATE TABLE competition_referee (
	competition integer, -- fk on competition
	referee integer,     -- fk on referee
	position integer     -- fk on position
) inherits (obj);

CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON competition_referee FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();

CREATE TABLE competition_member (
	competition integer,  -- fk on competition
	member integer,  -- fk on member
	role integer -- fk on role
) inherits (obj);

CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON competition_member FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();

-- матчи в командных соревнованиях
CREATE TABLE match (
	competition integer,        -- fk on competition
	num         integer default 0,        -- 
	mat         integer,        -- fk on mat
	cgroup      integer,        -- fk cgroup
	circle      integer,        -- fk on circle
	style       integer,        -- fk on style
	team_a      integer,        -- fk on team
	team_b      integer,        -- fk on team
	cround      integer,        -- fk on competition round
	ctour       integer,        -- fk on competition tour,
	winner      integer,        -- fk on team
	classify    integer,        -- fk on classify
	title       varchar,
	team_a_win  integer default 0,
	team_b_win  integer default 0
) inherits (obj);

CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON match FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();
CREATE TRIGGER _2_title_insert_update BEFORE INSERT OR UPDATE ON match FOR EACH ROW EXECUTE PROCEDURE update_match_title ();
CREATE TRIGGER _3_match_before_update BEFORE UPDATE ON match FOR EACH ROW EXECUTE PROCEDURE match_before_update ();
CREATE TRIGGER _4_match_after_update AFTER UPDATE ON match FOR EACH ROW EXECUTE PROCEDURE match_after_update ();

CREATE TRIGGER _10_remove_match BEFORE DELETE ON match FOR EACH ROW EXECUTE PROCEDURE remove_match ();

-- схватки 
CREATE TABLE fight (
	competition integer,        -- fk on competition
	match       integer,        -- fk on competition match
	style       integer,        -- fk on style
	weight      integer,        -- fk on weight
	circle      integer,        -- fk on circle
	cround      integer,        -- fk on competition round
	ctour       integer,        -- fk on competition tour
	mat         integer,        -- fk on mat
	num         integer default 0,                
	red         integer,        -- fk on wrestler
	blue        integer,        -- fk on wrestler
	winner      integer,        -- fk on wrestler
	start       timestamp,
	stop        timestamp,
	classify    integer,        -- fk on classify
	red_pen     int[] default '{0,0,0}',
	blue_pen    int[] default '{0,0,0}',
	red_score   int[] default '{0,0,0}',
	blue_score  int[] default '{0,0,0}',
	fstart      timestamp,
	fstop       timestamp,
	on_fight    boolean default false
) inherits (obj); 

CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON fight FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();
CREATE TRIGGER _8_fight_after_update_byteam AFTER UPDATE ON fight FOR EACH ROW EXECUTE PROCEDURE fight_update_byteam ();
CREATE TRIGGER _9_fight_after_update_personal AFTER UPDATE ON fight FOR EACH ROW EXECUTE PROCEDURE fight_update_personal ();
CREATE TRIGGER _10_remove_fight BEFORE DELETE ON fight FOR EACH ROW EXECUTE PROCEDURE remove_fight ();

CREATE TABLE fight_log (
	fight integer,         -- fk on fight
	wrestler integer,      -- fk on wrester
	action integer,        -- fk on action
	fround integer,        -- fk on around
	score integer,
	etime timestamp not null default now()
) inherits (obj);

CREATE TRIGGER _1_obj_insert_update BEFORE INSERT OR UPDATE ON fight_log FOR EACH ROW EXECUTE PROCEDURE obj_before_insert_update ();
CREATE TRIGGER _2_fight_log_after_update BEFORE INSERT OR UPDATE ON fight_log FOR EACH ROW EXECUTE PROCEDURE fight_log_after_update ();


