--users
CREATE TABLE users(
idu INTEGER PRIMARY KEY,
login varchar(60) NOT NULL,
password varchar(200) NOT NULL
);

--users' statistics
CREATE TABLE stats(
idu INTEGER NOT NULL,
victories INTEGER NOT NULL,
defeats INTEGER NOT NULL,
gained_territories INTEGER NOT NULL,
lost_territories INTEGER NOT NULL,
reinforcement INTEGER NOT NULL,
sets_of_tokens INTEGER NOT NULL,
CONSTRAINT pk_stats PRIMARY KEY(idu),
CONSTRAINT fk_stats_users FOREIGN KEY(idu) references users(idu) on delete cascade
);

--games
CREATE TABLE games(
idg INTEGER PRIMARY KEY,
nbplayers INTEGER NOT NULL,
map_name varchar(60) NOT NULL,
CONSTRAINT ck_nbplayers CHECK (nbplayers >1 and nbplayers <7)
);

--games ranking
CREATE TABLE ranking(
idg INTEGER,
idu INTEGER,
place INTEGER NOT NULL,
CONSTRAINT pk_ranking PRIMARY KEY (idg,idu),
CONSTRAINT fk_ranking_games FOREIGN KEY(idg) references games(idg) on delete cascade,
CONSTRAINT fk_ranking_users FOREIGN KEY(idu) references users(idu) on delete cascade
);
