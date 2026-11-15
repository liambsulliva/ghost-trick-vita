#include <string.h>
#include "script_tables.h"
#include "test_support.h"

#define CLOSE_ENOUGH 0.001f

static struct script_tables tables;
static const char effects[] = "pulse core_ring 70 0 64 0.65 3 91 1 44 131 0.105\npulse bad_order 70 0 1 1 5 2 1 5 3 1\npulse past_period 70 0 1 1 70 2 1\npulse no_keys 70\n";

static int is_near(float actual, float expected)
{
    float gap;

    gap = actual - expected;
    return gap < CLOSE_ENOUGH && gap > -CLOSE_ENOUGH;
}

static const struct script_pulse *read_pulses(void)
{
    memset(&tables, 0, sizeof(tables));
    script_tables_read_effects(&tables, effects, sizeof(effects) - 1);
    return script_tables_find_pulse(&tables, "core_ring");
}

static void test_a_pulse_line_reads_its_period_and_keys(void)
{
    const struct script_pulse *pulse;

    pulse = read_pulses();
    CHECK(pulse != NULL);
    CHECK_INT(pulse->period_ticks, 70);
    CHECK_INT(pulse->key_count, 3);
    CHECK_INT(pulse->keys[2].tick, 44);
}

static void test_a_pulse_with_keys_out_of_order_or_past_its_period_is_refused(void)
{
    read_pulses();
    CHECK(script_tables_find_pulse(&tables, "bad_order") == NULL);
    CHECK(script_tables_find_pulse(&tables, "past_period") == NULL);
    CHECK(script_tables_find_pulse(&tables, "no_keys") == NULL);
}

static void test_the_pulse_runs_between_its_keys_and_hides_after_the_last(void)
{
    const struct script_pulse *pulse;
    float size;
    float alpha;

    pulse = read_pulses();
    CHECK(script_pulse_at(pulse, 0, &size, &alpha));
    CHECK(is_near(size, 64.0f));
    CHECK(is_near(alpha, 0.65f));
    CHECK(script_pulse_at(pulse, 3, &size, &alpha));
    CHECK(is_near(size, 91.0f));
    CHECK(is_near(alpha, 1.0f));
    CHECK(script_pulse_at(pulse, 23, &size, &alpha));
    CHECK(size > 91.0f && size < 131.0f);
    CHECK(alpha < 1.0f && alpha > 0.105f);
    CHECK(script_pulse_at(pulse, 44, &size, &alpha));
    CHECK(is_near(size, 131.0f));
    CHECK(!script_pulse_at(pulse, 45, &size, &alpha));
    CHECK(!script_pulse_at(pulse, 69, &size, &alpha));
}

static void test_the_pulse_starts_again_every_period(void)
{
    const struct script_pulse *pulse;
    float size;
    float alpha;

    pulse = read_pulses();
    CHECK(script_pulse_at(pulse, 70, &size, &alpha));
    CHECK(is_near(size, 64.0f));
    CHECK(script_pulse_at(pulse, 143, &size, &alpha));
    CHECK(is_near(size, 91.0f));
}

int main(void)
{
    test_a_pulse_line_reads_its_period_and_keys();
    test_a_pulse_with_keys_out_of_order_or_past_its_period_is_refused();
    test_the_pulse_runs_between_its_keys_and_hides_after_the_last();
    test_the_pulse_starts_again_every_period();
    return test_finish("unit/effect_pulse");
}
