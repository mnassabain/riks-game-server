--users
CREATE TABLE users(
idu int(11) AUTO_INCREMENT,
login varchar(60) NOT NULL,
password varchar(200) NOT NULL,
constraint pk_users PRIMARY KEY (idu)
);

--users' statistics
CREATE TABLE stats(
idu int(11) NOT NULL,
victories int(11) NOT NULL,
defeats int(11) NOT NULL,
gained_territories int(11) NOT NULL,
lost_territories int(11) NOT NULL,
reinforcement int(11) NOT NULL,
sets_of_tokens int(11) NOT NULL,
CONSTRAINT pk_stats PRIMARY KEY(idu),
CONSTRAINT fk_stats_users FOREIGN KEY(idu) references users(idu) on delete cascade
);

--games
CREATE TABLE games(
idg int(11) AUTO_INCREMENT,
nbplayers int(11) NOT NULL,
map_name varchar(60) NOT NULL,
CONSTRAINT pk_games PRIMARY KEY(idg),
CONSTRAINT ck_nbplayers CHECK (nbplayers >1 and nbplayers <7)
);

--games ranking
CREATE TABLE ranking(
idg int(11),
idu int(11),
place int(11) NOT NULL,
CONSTRAINT pk_ranking PRIMARY KEY (idg,idu),
CONSTRAINT fk_ranking_games FOREIGN KEY(idg) references games(idg) on delete cascade,
CONSTRAINT fk_ranking_users FOREIGN KEY(idu) references users(idu) on delete cascade
);