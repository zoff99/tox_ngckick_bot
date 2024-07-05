#include "csorma_runtime.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef UNUSED
#elif defined(__GNUC__)
# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
# define UNUSED(x) /*@unused@*/ x
#else
# define UNUSED(x) x
#endif

int main()
{
    printf("TEST: CSORMA version: %s\n", csorma_get_version());
    printf("TEST: CSORMA SQLite version: %s\n", csorma_get_sqlite_version());
    printf("TEST: CSORMA sqlcipher version: %s\n", csorma_get_sqlcipher_version());

    // ----------- initialize DB -----------
    const char *db_dir = "./";
    const char *db_filename = "example.db";
    OrmaDatabase *o = OrmaDatabase_init((uint8_t*)db_dir,
            strlen(db_dir),
            (uint8_t*)db_filename, strlen(db_filename));
    // ----------- initialize DB -----------

#ifdef ENCRYPT_CSORMA
    printf("TEST: csorma was compiled with sqlcipher encryption\n");
#endif

    const char *key = "secret007$%";
    int r = OrmaDatabase_key(o, (uint8_t*)key, strlen(key));
    printf("TEST: setting sqlcipher key. result = %d\n", r);

    // ----------- freehand SQL to create TABLE -----------
    char *sql1 = "CREATE TABLE IF NOT EXISTS \"Message\" ("
  "\"id\" INTEGER,"
  "\"message_id\" INTEGER,"
  "\"tox_friendpubkey\" TEXT,"
  "\"direction\" INTEGER,"
  "\"tox_message_type\" INTEGER,"
  "\"trifa_message_type\" INTEGER,"
  "\"state\" INTEGER,"
  "\"ft_accepted\" BOOLEAN,"
  "\"ft_outgoing_started\" BOOLEAN,"
  "\"filedb_id\" INTEGER,"
  "\"filetransfer_id\" INTEGER,"
  "\"sent_timestamp\" INTEGER,"
  "\"sent_timestamp_ms\" INTEGER,"
  "\"rcvd_timestamp\" INTEGER,"
  "\"rcvd_timestamp_ms\" INTEGER,"
  "\"read\" BOOLEAN,"
  "\"send_retries\" INTEGER,"
  "\"is_new\" BOOLEAN,"
  "\"text\" TEXT,"
  "\"filename_fullpath\" TEXT,"
  "\"msg_id_hash\" TEXT,"
  "\"raw_msgv2_bytes\" TEXT,"
  "\"msg_version\" INTEGER,"
  "\"resend_count\" INTEGER,"
  "\"ft_outgoing_queued\" BOOLEAN,"
  "\"msg_at_relay\" BOOLEAN,"
  "\"msg_idv3_hash\" TEXT,"
  "\"sent_push\" INTEGER,"
  "\"filetransfer_kind\" INTEGER,"
  "PRIMARY KEY(\"id\" )"
");"
        "insert into message(message_id,tox_friendpubkey) values('123', '0aaaaaaaaaaaaaaaaaaa');"
        "insert into message(message_id,tox_friendpubkey) values('111', '00f03428wierwrwer09wi9qie');"
        "insert into message(message_id,read,tox_friendpubkey) values('344', true, '11f03528wierwrwer09wi9qie');"
        "insert into message(message_id,tox_friendpubkey) values('344', '11f03428wierwrwer09wi9qie');"
        "insert into message(message_id,tox_friendpubkey) values('344', '33f03428wierwrwer09wi9qie');"
        ;
    CSORMA_GENERIC_RESULT UNUSED(res1) = OrmaDatabase_run_multi_sql(o, (const uint8_t *)sql1);
    // ----------- freehand SQL to create TABLE -----------

    // ----------- freehand SQL to create TABLE -----------
char *sql2 = "CREATE TABLE IF NOT EXISTS \"Friendlist\" ("
"  \"tox_public_key_string\" TEXT,"
"  \"name\" TEXT,"
"  \"alias_name\" TEXT,"
"  \"friendlist_id\" INTEGER,"
"  \"status_message\" TEXT,"
  "\"read\" BOOLEAN,"
"  \"message_id\" INTEGER,"
"  \"tox_connection\" INTEGER,"
"  \"tox_connection_real\" INTEGER,"
"  \"tox_connection_on_off\" INTEGER,"
"  \"tox_connection_on_off_real\" INTEGER,"
"  \"tox_user_status\" INTEGER,"
"  \"avatar_pathname\" TEXT,"
"  \"avatar_filename\" TEXT,"
"  \"avatar_hex\" TEXT,"
"  \"avatar_hash_hex\" TEXT,"
"  \"avatar_update\" BOOLEAN,"
"  PRIMARY KEY(\"tox_public_key_string\")"
");"
        "insert into Friendlist(message_id,tox_public_key_string) values('123', 'aaaaaaaaaaaaaaaaaaa');"
        "insert into Friendlist(message_id,tox_public_key_string) values('111', '00f03428wierwrwer09wi9qie');"
        "insert into Friendlist(message_id,read,tox_public_key_string) values('344', true, '11f03428wierwrwer09wi9qie');"
        "insert into Friendlist(message_id,tox_public_key_string) values('344', '1xf03428wierwrwer09wi9qie');"
        "insert into Friendlist(message_id,tox_public_key_string) values('344', '33f03428wierwrwer09wi9qie');"
        ;
    CSORMA_GENERIC_RESULT UNUSED(res2) = OrmaDatabase_run_multi_sql(o, (const uint8_t *)sql2);
    // ----------- freehand SQL to create TABLE -----------

    // ----------- insert SQL -----------
    csorma_s *str1 = csb("abc!?%_\\");
    csorma_s *str2 = csc("test text", strlen("test text"));
    Message *m = orma_new_Message(o->db);
    m->tox_friendpubkey = str1;
    m->text = str2;
    m->message_id = 344;
    int64_t UNUSED(rowid) = orma_insertIntoMessage(m);
    orma_free_Message(m);
    // ----------- insert SQL -----------

    // ----------- count(*) SQL -----------
    Message *m4 = orma_selectFromMessage(o->db);
    printf("TEST: count m4: %d\n", (int)m4->message_idEq(m4, 344)->count(m4));
    // ----------- count(*) SQL -----------

    // ----------- select test -----------
    Message *m5 = orma_selectFromMessage(o->db);
    MessageList *ml = m5->message_idEq(m5, 344)->toList(m5);
    printf("TEST: ml->items=%ld\n", ml->items);
    Message **md = ml->l;
    for(int i=0;i<ml->items;i++)
    {
        printf("TEST: id=%ld\n", (*md)->id);
        printf("TEST: read=%d\n", (*md)->read);
        printf("TEST: mid=%ld\n", (*md)->message_id);
        printf("TEST: pk=\"%s\"\n", (*md)->tox_friendpubkey->s);
        printf("TEST: text=\"%s\"\n", (*md)->text->s);
        md++;
    }
    orma_free_MessageList(ml);
    // ----------- select test -----------

    printf("\n\n");
    printf("TEST: M6 select -------------------------------\n");

    // ----------- select test -----------
    {
    Message *mx1 = orma_selectFromMessage(o->db);
    MessageList *ml6 = mx1
                        ->readEq(mx1, true)
                        ->tox_friendpubkeyEq(mx1, csb("11f03428wierwrwer09wi9qie"))
                        ->message_idEq(mx1, 344)
                        ->toList(mx1);
    printf("TEST: ml6->items=%ld\n", ml6->items);
    Message **md6 = ml6->l;
    for(int i=0;i<ml6->items;i++)
    {
        printf("TEST: id=%ld\n", (*md6)->id);
        printf("TEST: mid=%ld\n", (*md6)->message_id);
        printf("TEST: pk=\"%s\"\n", (*md6)->tox_friendpubkey->s);
        printf("TEST: text=\"%s\"\n", (*md6)->text->s);
        md6++;
    }
    orma_free_MessageList(ml6);
    }
    // ----------- select test -----------

    printf("\n\n");

    // ----------- select test -----------
    {
    Message *mx1 = orma_selectFromMessage(o->db);
    MessageList *ml6 = mx1
                        ->readEq(mx1, true)
                        ->tox_friendpubkeyEq(mx1, csb("11f03428wierwrwer09wi9qie"))
                        ->message_idEq(mx1, 344)
                        ->toList(mx1);
    printf("TEST: ml7->items=%ld\n", ml6->items);
    Message **md6 = ml6->l;
    for(int i=0;i<ml6->items;i++)
    {
        printf("TEST: id=%ld\n", (*md6)->id);
        printf("TEST: mid=%ld\n", (*md6)->message_id);
        printf("TEST: pk=\"%s\"\n", (*md6)->tox_friendpubkey->s);
        printf("TEST: text=\"%s\"\n", (*md6)->text->s);
        md6++;
    }
    orma_free_MessageList(ml6);
    }
    // ----------- select test -----------

    printf("\n\n");

    Message *m99;
    Friendlist *f99;

    // ----------- count(*) SQL -----------
    m99 = orma_selectFromMessage(o->db);
    printf("TEST: count m: %d\n", (int)m99->readEq(m99, true)->count(m99));
    // ----------- count(*) SQL -----------

    // ----------- count(*) SQL -----------
    m99 = orma_selectFromMessage(o->db);
    printf("TEST: count m(like): %d\n", (int)m99->tox_friendpubkeyLike(m99, csb("11f035%"))->count(m99));
    // ----------- count(*) SQL -----------

    // ----------- count(*) SQL -----------
    m99 = orma_selectFromMessage(o->db);
    printf("TEST: count before update: %d\n", (int)m99->textEq(m99, csb("test123_$!"))->count(m99));
    // ----------- count(*) SQL -----------

    // ----------- update SQL -----------
    m99 = orma_updateMessage(o->db);
    int64_t affected_rows3 = m99
        // ->readEq(m99, false)
        ->message_idEq(m99, 123)
        ->textSet(m99, csb("test123_$!"))
        ->execute(m99);
    printf("TEST: affected rows: %d\n", (int)affected_rows3);
    // ----------- update SQL -----------

    // ----------- count(*) SQL -----------
    m99 = orma_selectFromMessage(o->db);
    printf("TEST: count after update: %d\n", (int)m99->textEq(m99, csb("test123_$!"))->count(m99));
    // ----------- count(*) SQL -----------




    // ----------- count(*) SQL -----------
    m99 = orma_selectFromMessage(o->db);
    printf("TEST: count before delete: %d\n", (int)m99->message_idEq(m99, 111)->count(m99));
    // ----------- count(*) SQL -----------

    // ----------- update SQL -----------
    m99 = orma_deleteFromMessage(o->db);
    int64_t affected_rows1 = m99
        ->message_idEq(m99, 111)
        ->execute(m99);
    printf("TEST: affected rows: %d\n", (int)affected_rows1);
    // ----------- update SQL -----------

    // ----------- count(*) SQL -----------
    m99 = orma_selectFromMessage(o->db);
    printf("TEST: count after delete: %d\n", (int)m99->message_idEq(m99, 111)->count(m99));
    // ----------- count(*) SQL -----------


    // ----------- count(*) SQL -----------
    f99 = orma_selectFromFriendlist(o->db);
    printf("TEST: count before delete: %d\n", (int)f99->friendlist_idEq(f99, 111)->count(f99));
    // ----------- count(*) SQL -----------

    // ----------- update SQL -----------
    f99 = orma_deleteFromFriendlist(o->db);
    int64_t affected_rows2 = f99
        ->friendlist_idEq(f99, 111)
        ->execute(f99);
    printf("TEST: affected rows: %d\n", (int)affected_rows2);
    // ----------- update SQL -----------

    // ----------- count(*) SQL -----------
    f99 = orma_selectFromFriendlist(o->db);
    printf("TEST: count after delete: %d\n", (int)f99->friendlist_idEq(f99, 111)->count(f99));
    // ----------- count(*) SQL -----------



    // ----------- insert utf-8 and just binary -----------
    printf("===================================\n");
    printf("TEST: broken 001 (insert just bytes)\n");
    {
    int len_broken = 20;
    char *broken = (char *)calloc(1, len_broken);
    char *ptr = broken;
    *ptr = (char)120; ptr++;
    *ptr = (char)10; ptr++;
    *ptr = (char)65; ptr++;
    *ptr = (char)66; ptr++;
    *ptr = (char)3; ptr++;
    *ptr = (char)6; ptr++;
    *ptr = (char)170; ptr++;

    *ptr = (char)0xF0; ptr++;
    *ptr = (char)0x80; ptr++;
    *ptr = (char)0x82; ptr++;
    *ptr = (char)0x80; ptr++;

    *ptr = (char)0; ptr++;
    *ptr = (char)65; ptr++;
    *ptr = (char)65; ptr++;
    *ptr = (char)65; ptr++;
    csorma_s *str2 = csc(broken, len_broken);

    printf("TEST: str2 text=\"%s\"\n", str2->s);
    printf("TEST: str2 text bytes=\"%d\"\n", str2->l);
    printf("TEST: str2 text null term=\"%d\"\n", str2->n);

    free(broken);
    Message *m = orma_new_Message(o->db);
    m->tox_friendpubkey = csb("test_broken_001");
    m->text = str2;
    m->message_id = 123000010;
    int64_t UNUSED(rowid) = orma_insertIntoMessage(m);
    orma_free_Message(m);
    }
    // ----------- insert utf-8 and just binary -----------
    // ----------- select utf-8 and just binary -----------
    {
    Message *mx1 = orma_selectFromMessage(o->db);
    MessageList *ml6 = mx1
                        ->message_idEq(mx1, 123000010)
                        ->toList(mx1);
    printf("TEST: ml7->items=%ld\n", ml6->items);
    Message **md6 = ml6->l;
    for(int i=0;i<ml6->items;i++)
    {
        printf("TEST: id=%ld\n", (*md6)->id);
        printf("TEST: mid=%ld\n", (*md6)->message_id);
        printf("TEST: pk=\"%s\"\n", (*md6)->tox_friendpubkey->s);
        printf("TEST: text=\"%s\"\n", (*md6)->text->s);
        printf("TEST: text bytes=\"%d\"\n", (*md6)->text->l);
        printf("TEST: text null term=\"%d\"\n", (*md6)->text->n);
        md6++;
    }
    orma_free_MessageList(ml6);
    }
    printf("===================================\n");
    // ----------- select utf-8 and just binary -----------


    // ----------- insert large buffer with random bytes -----------
    printf("===================================\n");
    printf("TEST: broken 002 (large buffer with random bytes)\n");
    const int ll = 300000;
    unsigned char buffer[ll];
    {
    int i;
    srand(time(NULL));
    for (i = 0; i < ll; i++) {
        unsigned char c = rand() % 256;
        if (c == 0) { c = 1; }
        buffer[i] = c;
    }

    csorma_s *str2 = csc((const char *)buffer, ll);

    Message *m = orma_new_Message(o->db);
    m->tox_friendpubkey = csb("test_broken_002");
    m->text = str2;
    m->message_id = 123000020;
    int64_t UNUSED(rowid) = orma_insertIntoMessage(m);
    orma_free_Message(m);
    }

    {
    Message *mx1 = orma_selectFromMessage(o->db);
    MessageList *ml6 = mx1
                        ->message_idEq(mx1, 123000020)
                        ->toList(mx1);
    printf("TEST: ml7->items=%ld\n", ml6->items);
    Message **md6 = ml6->l;
    for(int i=0;i<ml6->items;i++)
    {
        printf("TEST: id=%ld\n", (*md6)->id);
        printf("TEST: mid=%ld\n", (*md6)->message_id);
        printf("TEST: pk=\"%s\"\n", (*md6)->tox_friendpubkey->s);
        // printf("TEST: text=\"%s\"\n", (*md6)->text->s);

        if (ll != (*md6)->text->l)
        {
            printf("TEST: ERR-111: len differs %d != %d\n", ll, (*md6)->text->l);
            exit(-111);
        }
        for (i = 0; i < ll; i++) {
            if ((uint8_t)buffer[i] != (uint8_t)(*md6)->text->s[i]) {
                printf("TEST: ERR-112: text diff at position %d\n", i);
                exit(-112);
            }
        }
        printf("TEST: bytes inserted is equal to bytes selected\n");

        printf("TEST: text bytes=\"%d\"\n", (*md6)->text->l);
        printf("TEST: text null term=\"%d\"\n", (*md6)->text->n);
        md6++;
    }
    orma_free_MessageList(ml6);
    }
    printf("===================================\n");
    // ----------- insert large buffer with random bytes -----------



    // ----------- freehand SQL -----------
    char *sql3 = "DROP TABLE Message;";
    CSORMA_GENERIC_RESULT res3 = OrmaDatabase_run_multi_sql(o, (const uint8_t *)sql3);
    printf("TEST: res3: %d\n", res3);
    // ----------- freehand SQL -----------

    // ----------- shutdown DB -----------
    OrmaDatabase_shutdown(o);
    // ----------- shutdown DB -----------

    printf("\n");
    printf("TEST: all OK\n");
    printf("\n");

    return 0;
}

#ifdef __cplusplus
}  // extern "C"
#endif
