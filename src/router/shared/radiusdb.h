#ifndef _radiusdb_h_
#define _radiusdb_h_

struct radiususer {
	unsigned int fieldlen;
	unsigned int usersize;
	unsigned char *user;
	unsigned int passwordsize;
	unsigned char *passwd;
	unsigned int downstream;
	unsigned int upstream;
	unsigned int expiration;
//more fields can be added in future
};

struct radiusdb {
	unsigned int usercount;
	struct radiususer *users;
};

struct radiusclient {
	unsigned int fieldlen;
	unsigned int clientsize;
	unsigned char *client;
	unsigned int passwordsize;
	unsigned char *passwd;
//more fields can be added in future
};

struct radiusclientdb {
	unsigned int usercount;
	struct radiusclient *users;
};

struct radiusdb *loadradiusdb(void);
void writeradiusdb(struct radiusdb *db);
void freeradiusdb(struct radiusdb *db);

struct radiusclientdb *loadradiusclientdb(void);
void writeradiusclientdb(struct radiusclientdb *db);
void freeradiusclientdb(struct radiusclientdb *db);

#define TYPE_SERVER 0x01
#define TYPE_CA 0x2
#define TYPE_CLIENT 0x3

void gen_cert(char *name, int type, char *common, char *pass);

#endif
