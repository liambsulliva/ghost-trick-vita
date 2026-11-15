#include <string.h>
#include "script_tables.h"
#include "test_support.h"

#define OVAL_KIND 0
#define THORN_KIND 2
#define ALERT_KIND 5

static struct script_tables tables;
static const char balloons[] =
    "balloon 0 said 0\npart oval_shape 0 0 128 64 0 8\n"
    "balloon 2 said 0\npart thorn_shape 0 0 128 64 0 8\n"
    "balloon -1 plain 0\npart plain_shape 0 0 128 64 0 8\n"
    "balloon 5 - 11\npart alert_shape 0 0 28 28 0 3\n";

static void test_a_label_baked_for_two_kinds_finds_the_row_of_its_kind(void)
{
    const struct script_balloon_row *row;

    row = script_tables_find_balloon(&tables, "said", OVAL_KIND);
    CHECK(row != NULL && strcmp(row->parts[0].texture_name, "oval_shape") == 0);
    row = script_tables_find_balloon(&tables, "said", THORN_KIND);
    CHECK(row != NULL && strcmp(row->parts[0].texture_name, "thorn_shape") == 0);
    CHECK(script_tables_find_balloon(&tables, "said", ALERT_KIND) == NULL);
}

static void test_a_row_with_no_kind_serves_every_kind(void)
{
    const struct script_balloon_row *row;

    row = script_tables_find_balloon(&tables, "plain", THORN_KIND);
    CHECK(row != NULL && strcmp(row->parts[0].texture_name, "plain_shape") == 0);
    row = script_tables_find_balloon(&tables, "plain", OVAL_KIND);
    CHECK(row != NULL && strcmp(row->parts[0].texture_name, "plain_shape") == 0);
}

static void test_a_balloon_with_no_label_is_found_by_its_kind(void)
{
    const struct script_balloon_row *row;

    row = script_tables_find_balloon(&tables, NULL, ALERT_KIND);
    CHECK(row != NULL && strcmp(row->parts[0].texture_name, "alert_shape") == 0);
    CHECK(script_tables_find_balloon(&tables, NULL, OVAL_KIND) == NULL);
}

int main(void)
{
    memset(&tables, 0, sizeof(tables));
    CHECK(script_tables_read_balloons(&tables, balloons, sizeof(balloons) - 1));
    test_a_label_baked_for_two_kinds_finds_the_row_of_its_kind();
    test_a_row_with_no_kind_serves_every_kind();
    test_a_balloon_with_no_label_is_found_by_its_kind();
    return test_finish("unit/balloon_row_kinds");
}
