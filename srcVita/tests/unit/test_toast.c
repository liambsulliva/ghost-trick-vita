#include <string.h>
#include "fakes/fake_renderer.h"
#include "pause_widgets.h"
#include "test_support.h"
#include "toast.h"

#define FRAME_US 16667ull

static void run_for(unsigned long long us)
{
    unsigned long long done;

    for (done = 0; done < us; done += FRAME_US) {
        toast_update(FRAME_US);
    }
}

static void test_a_toast_slides_down_holds_then_leaves(void)
{
    static const char table[] = "text bar.new_illustrations 0 0 300 28\n";

    CHECK_INT(pause_widgets_read_text(table, (unsigned int)strlen(table)), 1);
    CHECK(!toast_is_showing());
    CHECK(toast_top() < 0.0f);
    toast_show("bar.new_illustrations");
    CHECK(toast_is_showing());
    CHECK(toast_top() < 0.0f);
    run_for(400000ull);
    CHECK(toast_top() == 12.0f);
    run_for(2000000ull);
    CHECK(toast_top() == 12.0f);
    run_for(1000000ull);
    CHECK(toast_top() < 12.0f);
    run_for(400000ull);
    CHECK(!toast_is_showing());
}

int main(void)
{
    fake_renderer_reset();
    test_a_toast_slides_down_holds_then_leaves();
    return test_finish("unit/toast");
}
