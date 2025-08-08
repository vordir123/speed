#pragma once
#include <FlexCAN_T4.h>

// Simple logistic regression style model for CAN message identification
// Features: message ID and first data byte.
// Weights are placeholders for demonstration.
static const float WEIGHTS[2] = {0.001f, 0.05f};
static const float BIAS = -1.0f;

inline int identifyMessage(const CAN_message_t &msg) {
    float feature0 = (float)msg.id;
    float feature1 = msg.len > 0 ? (float)msg.buf[0] : 0.0f;
    float score = feature0 * WEIGHTS[0] + feature1 * WEIGHTS[1] + BIAS;
    return score > 0 ? 1 : 0; // 1 indicates positive identification
}
