#include "fakes/fake_platform.h"
#include "fakes/fake_reader.h"
#include "texture_cache.h"
#include "test_support.h"

/* Root cause 299: the name index's links were -1 for none, so before texture_cache_init the zeroed
   index read as entry 0 linked to itself and the first lookup never returned. A view test that skipped
   the init hung the whole test run. */
static void test_a_lookup_before_the_cache_starts_returns(void)
{
    const struct cached_texture *first;
    const struct cached_texture *again;

    fake_files_reset();
    fake_reader_reset();
    first = texture_cache_use_or_start_loading("never_started");
    again = texture_cache_use_or_start_loading("never_started");
    CHECK(again == first);
    CHECK(texture_cache_use_or_start_loading("another_name") != first);
}

int main(void)
{
    test_a_lookup_before_the_cache_starts_returns();
    return test_finish("unit/texture_cache_before_init");
}
