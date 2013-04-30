-- indexes.sql

ALTER TABLE geo ADD CONSTRAINT geo_pk_idx PRIMARY KEY (id);
CREATE INDEX geo_trail_idx ON geo USING GIST (trail);

ALTER TABLE gender   ADD CONSTRAINT gender_pk_idx PRIMARY KEY (id);

ALTER TABLE style    ADD CONSTRAINT style_pk_idx PRIMARY KEY (id);

ALTER TABLE age      ADD CONSTRAINT age_pk_idx PRIMARY KEY (id);

ALTER TABLE weight   ADD CONSTRAINT weight_pk_idx PRIMARY KEY (id);

ALTER TABLE rang     ADD CONSTRAINT rang_pk_idx PRIMARY KEY (id);

ALTER TABLE position ADD CONSTRAINT position_pk_idx PRIMARY KEY (id);

ALTER TABLE ctour    ADD CONSTRAINT ctour_pk_idx PRIMARY KEY (id);

ALTER TABLE cround   ADD CONSTRAINT cround_pk_idx PRIMARY KEY (id);

ALTER TABLE fround   ADD CONSTRAINT fround_pk_idx PRIMARY KEY (id);

ALTER TABLE comptype  ADD CONSTRAINT comptype_pk_idx PRIMARY KEY (id);

ALTER TABLE action   ADD CONSTRAINT action_pk_idx PRIMARY KEY (id);

ALTER TABLE mat     ADD CONSTRAINT mat_pk_idx PRIMARY KEY (id);

ALTER TABLE classify ADD CONSTRAINT _pk_idx PRIMARY KEY (id);

ALTER TABLE role ADD CONSTRAINT role_pk_idx PRIMARY KEY (id);

ALTER TABLE circle ADD CONSTRAINT circle_pk_idx PRIMARY KEY (id);

ALTER TABLE contact ADD CONSTRAINT contact_pk_idx PRIMARY KEY (id);
ALTER TABLE contact ADD CONSTRAINT contact_gender_fk_idx FOREIGN KEY (gender) REFERENCES gender (id);

ALTER TABLE addresstype ADD CONSTRAINT addresstype_pk_idx PRIMARY KEY (id);

ALTER TABLE address ADD CONSTRAINT address_pk_idx PRIMARY KEY (id);

ALTER TABLE cgroup ADD CONSTRAINT cgroup_pk_idx PRIMARY KEY (id);

ALTER TABLE team ADD CONSTRAINT team_pk_idx PRIMARY KEY (id);
ALTER TABLE team ADD CONSTRAINT team_style_fk_idx FOREIGN KEY (style) REFERENCES style (id);
ALTER TABLE team ADD CONSTRAINT team_geo_fk_idx   FOREIGN KEY (geo)   REFERENCES geo (id);

ALTER TABLE coach ADD CONSTRAINT coach_pk_idx PRIMARY KEY (id);
ALTER TABLE coach ADD CONSTRAINT coach_gender_fk_idx FOREIGN KEY (gender) REFERENCES gender (id);
ALTER TABLE coach ADD CONSTRAINT coach_rang_fk_idx FOREIGN KEY (rang) REFERENCES rang (id);
ALTER TABLE coach ADD CONSTRAINT coach_team_fk_idx FOREIGN KEY (team) REFERENCES team (id);

ALTER TABLE coach_address ADD CONSTRAINT coach_address_pk_idx PRIMARY KEY (id);
ALTER TABLE coach_address ADD CONSTRAINT coach_address_contact_fk_idx FOREIGN KEY (contact) REFERENCES coach (id);
ALTER TABLE coach_address ADD CONSTRAINT coach_address_addresstype_fk_idx FOREIGN KEY (addresstype) REFERENCES addresstype (id);

ALTER TABLE wrestler ADD CONSTRAINT wrestler_pk_idx PRIMARY KEY (id);
ALTER TABLE wrestler ADD CONSTRAINT wrestler_gender_fk_idx FOREIGN KEY (gender) REFERENCES gender (id);
ALTER TABLE wrestler ADD CONSTRAINT wrestler_rang_fk_idx   FOREIGN KEY (rang)   REFERENCES rang (id);
ALTER TABLE wrestler ADD CONSTRAINT wrestler_coach_fk_idx  FOREIGN KEY (coach)  REFERENCES coach (id);
ALTER TABLE wrestler ADD CONSTRAINT wrestler_geo_fk_idx  FOREIGN KEY (geo)  REFERENCES geo (id);

ALTER TABLE wrestler_address ADD CONSTRAINT wrestler_address_pk_idx PRIMARY KEY (id);
ALTER TABLE wrestler_address ADD CONSTRAINT wrestler_address_contact_fk_idx FOREIGN KEY (contact) REFERENCES wrestler (id);
ALTER TABLE wrestler_address ADD CONSTRAINT wrestler_address_addresstype_fk_idx FOREIGN KEY (addresstype) REFERENCES addresstype (id);

ALTER TABLE referee ADD CONSTRAINT referee_pk_idx PRIMARY KEY (id);
ALTER TABLE referee ADD CONSTRAINT referee_gender_fk_idx FOREIGN KEY (gender) REFERENCES gender (id);
ALTER TABLE referee ADD CONSTRAINT referee_rang_fk_idx   FOREIGN KEY (rang)   REFERENCES rang (id);
ALTER TABLE referee ADD CONSTRAINT referee_geo_fk_idx   FOREIGN KEY (geo)   REFERENCES geo (id);

ALTER TABLE referee_address ADD CONSTRAINT referee_address_pk_idx PRIMARY KEY (id);
ALTER TABLE referee_address ADD CONSTRAINT referee_address_contact_fk_idx FOREIGN KEY (contact) REFERENCES referee (id);
ALTER TABLE referee_address ADD CONSTRAINT referee_address_addresstype_fk_idx FOREIGN KEY (addresstype) REFERENCES addresstype (id);

ALTER TABLE member ADD CONSTRAINT member_pk_idx PRIMARY KEY (id);
ALTER TABLE member ADD CONSTRAINT member_gender_fk_idx FOREIGN KEY (gender) REFERENCES gender (id);

ALTER TABLE member_address ADD CONSTRAINT member_address_pk_idx PRIMARY KEY (id);
ALTER TABLE member_address ADD CONSTRAINT member_address_contact_fk_idx FOREIGN KEY (contact) REFERENCES referee (id);
ALTER TABLE member_address ADD CONSTRAINT member_address_addresstype_fk_idx FOREIGN KEY (addresstype) REFERENCES addresstype (id);


ALTER TABLE competition ADD CONSTRAINT competition_pk_idx PRIMARY KEY (id);
ALTER TABLE competition ADD CONSTRAINT competition_geo_fk_idx      FOREIGN KEY (geo)      REFERENCES geo (id);
ALTER TABLE competition ADD CONSTRAINT competition_age_fk_idx      FOREIGN KEY (age)      REFERENCES age (id);
ALTER TABLE competition ADD CONSTRAINT competition_comptype_fk_idx    FOREIGN KEY (comptype)    REFERENCES comptype (id);

ALTER TABLE competition_style ADD CONSTRAINT competition_style_pk_idx PRIMARY KEY (id);
ALTER TABLE competition_style ADD CONSTRAINT competition_style_competition_fk_idx FOREIGN KEY (competition) REFERENCES competition (id);
ALTER TABLE competition_style ADD CONSTRAINT competition_style_style_fk_idx FOREIGN KEY (style) REFERENCES style (id);

ALTER TABLE competition_weight ADD CONSTRAINT competition_weight_pk_idx PRIMARY KEY (id);
ALTER TABLE competition_weight ADD CONSTRAINT competition_weight_competition_fk_idx FOREIGN KEY (competition) REFERENCES competition (id);
ALTER TABLE competition_weight ADD CONSTRAINT competition_weight_weight_fk_idx FOREIGN KEY (weight) REFERENCES weight (id);

ALTER TABLE competition_team ADD CONSTRAINT competition_team_pk_idx PRIMARY KEY (id);
ALTER TABLE competition_team ADD CONSTRAINT competition_team_competition_fk_idx FOREIGN KEY (competition) REFERENCES competition (id);
ALTER TABLE competition_team ADD CONSTRAINT competition_team_team_fk_idx FOREIGN KEY (team) REFERENCES team (id);
ALTER TABLE competition_team ADD CONSTRAINT competition_team_style_fk_idx FOREIGN KEY (style) REFERENCES style (id);
ALTER TABLE competition_team ADD CONSTRAINT competition_team_cgroup_fk_idx FOREIGN KEY (cgroup) REFERENCES cgroup (id);

ALTER TABLE competition_wrestler ADD CONSTRAINT competition_wrestler_pk_idx PRIMARY KEY (id);
ALTER TABLE competition_wrestler ADD CONSTRAINT competition_wrestler_competition_fk_idx FOREIGN KEY (competition) REFERENCES competition (id);
ALTER TABLE competition_wrestler ADD CONSTRAINT competition_wrestler_team_fk_idx FOREIGN KEY (team) REFERENCES team (id);
ALTER TABLE competition_wrestler ADD CONSTRAINT competition_wrestler_wrestler_fk_idx FOREIGN KEY (wrestler) REFERENCES wrestler (id);
ALTER TABLE competition_wrestler ADD CONSTRAINT competition_wrestler_style_fk_idx FOREIGN KEY (style) REFERENCES style (id);
ALTER TABLE competition_wrestler ADD CONSTRAINT competition_wrestler_weight_fk_idx FOREIGN KEY (weight) REFERENCES weight (id);

ALTER TABLE competition_referee ADD CONSTRAINT competition_referee_pk_idx PRIMARY KEY (id);
ALTER TABLE competition_referee ADD CONSTRAINT competition_referee_competition_fk_idx FOREIGN KEY (competition) REFERENCES competition (id);
ALTER TABLE competition_referee ADD CONSTRAINT competition_referee_referee_fk_idx FOREIGN KEY (referee) REFERENCES referee (id);
ALTER TABLE competition_referee ADD CONSTRAINT competition_referee_position_fk_idx FOREIGN KEY (position) REFERENCES position (id);

ALTER TABLE competition_member ADD CONSTRAINT competition_member_pk_idx PRIMARY KEY (id);
ALTER TABLE competition_member ADD CONSTRAINT competition_member_competition_fk_idx FOREIGN KEY (competition) REFERENCES competition (id);
ALTER TABLE competition_member ADD CONSTRAINT competition_member_member_fk_idx FOREIGN KEY (member) REFERENCES member (id);
ALTER TABLE competition_member ADD CONSTRAINT competition_member_role_fk_idx FOREIGN KEY (role) REFERENCES role (id);

ALTER TABLE match ADD CONSTRAINT match_pk_idx PRIMARY KEY (id);
ALTER TABLE match ADD CONSTRAINT match_competition_fk_idx FOREIGN KEY (competition) REFERENCES competition (id);
ALTER TABLE match ADD CONSTRAINT match_circle_fk_idx FOREIGN KEY (circle) REFERENCES circle (id);
ALTER TABLE match ADD CONSTRAINT match_cgroup_fk_idx FOREIGN KEY (cgroup) REFERENCES cgroup (id);
ALTER TABLE match ADD CONSTRAINT match_team_a_fk_idx FOREIGN KEY (team_a) REFERENCES team (id);
ALTER TABLE match ADD CONSTRAINT match_team_b_fk_idx FOREIGN KEY (team_b) REFERENCES team (id);
ALTER TABLE match ADD CONSTRAINT match_style_fk_idx FOREIGN KEY (style) REFERENCES style (id);
ALTER TABLE match ADD CONSTRAINT match_cround_fk_idx FOREIGN KEY (cround) REFERENCES cround (id);
ALTER TABLE match ADD CONSTRAINT match_ctour_fk_idx FOREIGN KEY (ctour) REFERENCES ctour (id);
ALTER TABLE match ADD CONSTRAINT match_mat_fk_idx FOREIGN KEY (mat) REFERENCES mat (id);
ALTER TABLE match ADD CONSTRAINT match_winner_fk_idx FOREIGN KEY (winner) REFERENCES team (id);
ALTER TABLE match ADD CONSTRAINT match_classify_fk_idx FOREIGN KEY (classify) REFERENCES classify (id);

ALTER TABLE fight ADD CONSTRAINT fight_pk_idx PRIMARY KEY (id);
ALTER TABLE fight ADD CONSTRAINT fight_competition_fk_idx   FOREIGN KEY (competition)  REFERENCES competition (id);
ALTER TABLE fight ADD CONSTRAINT fight_style_fk_idx         FOREIGN KEY (style)        REFERENCES style (id);
ALTER TABLE fight ADD CONSTRAINT fight_weight_fk_idx        FOREIGN KEY (weight)       REFERENCES weight (id);
ALTER TABLE fight ADD CONSTRAINT fight_match_fk_idx         FOREIGN KEY (match)        REFERENCES match (id);
ALTER TABLE fight ADD CONSTRAINT fight_mat_fk_idx           FOREIGN KEY (mat)          REFERENCES mat (id);
ALTER TABLE fight ADD CONSTRAINT fight_red_fk_idx           FOREIGN KEY (red)          REFERENCES wrestler (id);
ALTER TABLE fight ADD CONSTRAINT fight_blue_fk_idx          FOREIGN KEY (blue)         REFERENCES wrestler (id);
ALTER TABLE fight ADD CONSTRAINT fight_classify_fk_idx      FOREIGN KEY (classify)     REFERENCES classify (id);
ALTER TABLE fight ADD CONSTRAINT fight_cround_fk_idx        FOREIGN KEY (cround)       REFERENCES cround (id);
ALTER TABLE fight ADD CONSTRAINT fight_ctour_fk_idx         FOREIGN KEY (ctour)        REFERENCES ctour (id);

ALTER TABLE fight_log ADD CONSTRAINT fight_log_pk_idx PRIMARY KEY (id);
ALTER TABLE fight_log ADD CONSTRAINT fight_log_fight_fk_idx    FOREIGN KEY (fight)    REFERENCES fight (id);
ALTER TABLE fight_log ADD CONSTRAINT fight_log_wrestler_fk_idx FOREIGN KEY (wrestler) REFERENCES wrestler (id);
ALTER TABLE fight_log ADD CONSTRAINT fight_log_action_fk_idx   FOREIGN KEY (action)   REFERENCES action (id);
ALTER TABLE fight_log ADD CONSTRAINT fight_log_fround_fk_idx   FOREIGN KEY (fround)   REFERENCES fround (id);