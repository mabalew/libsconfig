DROP TABLE config;
CREATE TABLE config (
  application VARCHAR NOT NULL DEFAULT 'DEFAULT_APPLICATION',
  context VARCHAR NOT NULL DEFAULT 'DEFAULT_CONTEXT',
  key VARCHAR,
  value VARCHAR NOT NULL DEFAULT '',
  temporary INTEGER NOT NULL DEFAULT '0',
  PRIMARY KEY(application, context, key)
);
CREATE INDEX idx_config ON config(application, context, key);

INSERT INTO config(key, value, temporary) VALUES('v2', 'k2', 0);

SELECT * FROM config;
