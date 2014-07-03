DROP TABLE config;
CREATE TABLE config (
  application VARCHAR NOT NULL DEFAULT 'DEFAULT_APPLICATION',
  context VARCHAR NOT NULL DEFAULT 'DEFAULT_CONTEXT',
  key VARCHAR,
  value VARCHAR NOT NULL DEFAULT '',
  PRIMARY KEY(application, context, key)
);
CREATE INDEX idx_config ON config(application, context, key);

INSERT INTO config VALUES(NULL, NULL, 'v2', 'k2');

SELECT * FROM config;
