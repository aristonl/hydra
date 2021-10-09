#pragma once

struct InterruptFrame;
__attribute__((interrupt)) void PageFaultHandler(struct InterruptFrame* frame);