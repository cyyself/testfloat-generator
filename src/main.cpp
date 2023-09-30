#include <cstdio>
#include <queue>
#include <cassert>
#include <cstdint>

extern "C" {
    #include "functions.h"
    #include "softfloat.h"
    #include "genCases.h"
    #include "genLoops.h"
}

struct testdata {
    uint64_t a;
    uint64_t b;
    uint64_t expected_out;
    function_t function;
    roundingMode_t roundingMode;
    exceptionFlag_t expectedException;
};

std::queue <testdata> test_queue;

bool dpiNextTest(/* TODO: args TBD */) { // if return false, the test ends
    if (test_queue.empty()) return false;
    testdata curr = test_queue.front();
    test_queue.pop();
    /* TODO: Send curr to DPI TBD */
    return true;
}

std::vector<testdata> mygen_abz_f32( float32_t trueFunction( float32_t, float32_t ) , function_t function, roundingMode_t roundingMode) {
    // modified from berkeley-testfloat-3/source/genLoops.c
    union ui32_f32 { uint32_t ui; float32_t f; } u;
    uint_fast8_t trueFlags;

    std::vector<testdata> res;

    genCases_f32_ab_init();
    while ( ! genCases_done ) {
        genCases_f32_ab_next();

        testdata curData;
        curData.function = function;
        curData.roundingMode = roundingMode;
        u.f = genCases_f32_a;
        curData.a = u.ui;
        u.f = genCases_f32_b;
        curData.b = u.ui;
        softfloat_exceptionFlags = 0;
        u.f = trueFunction( genCases_f32_a, genCases_f32_b );
        curData.expectedException = static_cast<exceptionFlag_t>(softfloat_exceptionFlags);
        curData.expected_out = u.ui;

        res.push_back(curData);
    }

    return res;
}

std::vector<testdata> genTestCase(function_t function, roundingMode_t roundingMode) { // call it in dpiInit
    // see berkeley-testfloat-3/source/testfloat_gen.c
    std::vector<testdata> res;

    genCases_setLevel( 1 );

    switch (function) {
        case F32_ADD:
            res = mygen_abz_f32(f32_add, function, roundingMode);
            break;
        default:
            assert(false);
    }

    return res;
}

void outputTestCases(std::vector<testdata> cases) {
    for (auto x : cases) {
        printf("%08x %08x %08x %02x\n", x.a, x.b, x.expected_out, x.expectedException);
    }
}

void fillTestQueue(std::vector<testdata> cases) {
    for (auto x : cases) {
        test_queue.push(x);
    }
}

void initTestCases() {
    auto res = genTestCase(F32_ADD, ROUND_NEAR_EVEN); 
    fillTestQueue(res);
    outputTestCases(res); // TODO: demo, please delete
}

int main() {
    initTestCases();
    return 0;
}