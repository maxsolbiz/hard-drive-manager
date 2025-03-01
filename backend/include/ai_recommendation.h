#ifndef AI_RECOMMENDATION_H
#define AI_RECOMMENDATION_H

#include <string>
#include "drive_health_monitor.h"

/**
 * @brief The AIRecommendation class encapsulates functions to provide
 * AI-based recommendations for drive health.
 *
 * This module calls an external Python script to generate recommendations
 * based on drive health metrics.
 */
class AIRecommendation {
public:
    /**
     * @brief Gets an AI-based recommendation for the given drive health info.
     *
     * @param healthInfo The drive health metrics.
     * @return A recommendation string, e.g., "Replace Drive Soon", "Backup Data Immediately", etc.
     */
    static std::string getRecommendation(const DriveHealthInfo &healthInfo);
};

#endif // AI_RECOMMENDATION_H
