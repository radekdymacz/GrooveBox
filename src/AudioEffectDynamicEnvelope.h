#include <AudioStream.h>

#define MAX_STAGES 64

class AudioEffectDynamicEnvelope : public AudioStream {
public:

    struct Stage {
        float targetLevel;
        float duration;
        float curveParam;   // Value between 0.0 to 1.0
    };

    AudioEffectDynamicEnvelope() : AudioStream(1, inputQueueArray) {
        reset();
    }
   
    void setStages(Stage *stages, int numStages) {
        if (numStages > MAX_STAGES) numStages = MAX_STAGES;
        for (int i = 0; i < numStages; i++) {
            this->stages[i] = stages[i];
        }
        this->numStages = numStages;
    }

    void start() {
        currentStage = 0;
        currentDuration = 0;
        currentLevel = 0.0;
        active = true;
    }

    void stop() {
        active = false;
    }

    void randomize(float totalLengthMs, int numStages) {
        if (numStages > MAX_STAGES) numStages = MAX_STAGES;

        float averageDuration = totalLengthMs / numStages;

        for (int i = 0; i < numStages; i++) {
            stages[i].targetLevel = random(0, 1000) / 1000.0;
            stages[i].duration = averageDuration + random(-averageDuration / 4, averageDuration / 4);

            // Randomly select curve parameter
            stages[i].curveParam = random(0, 1000) / 1000.0;  // Random float between 0.0 and 1.0

            totalLengthMs -= stages[i].duration;
            if (i == numStages - 1 && totalLengthMs > 0) {
                stages[i].duration += totalLengthMs;
            }
        }

        this->numStages = numStages;
    }

    void addStage(float targetLevel, float duration, float curveParam) {
        if (numStages >= MAX_STAGES) {
            // If we're at maximum capacity, just return.
            return;
        }

        stages[numStages].targetLevel = targetLevel;
        stages[numStages].duration = duration;
        stages[numStages].curveParam = curveParam;

        numStages++;
    }
    
    void visualize() {
        const int maxBarLength = 50;  // Adjust for longer/shorter bars

        Serial.println("Envelope Visualization:");

        for (int i = 0; i < numStages; i++) {
            int barLength = (int)(stages[i].targetLevel * maxBarLength);

            Serial.print("Stage ");
            Serial.print(i + 1);
            Serial.print(": [");
            
            // Print the bar using '#' character
            for (int j = 0; j < barLength; j++) {
                Serial.print("#");
            }

            // Fill the remaining space with spaces
            for (int j = barLength; j < maxBarLength; j++) {
                Serial.print(" ");
            }
            Serial.print(stages[i].curveParam < 0.5f ? "L" : "E");    
            Serial.println("]");

        }

        Serial.println("End of Visualization.");
    }

    virtual void update(void);

private:
    audio_block_t *inputQueueArray[1];
    Stage stages[MAX_STAGES];
    int numStages = 0;
    int currentStage = 0;
    float currentDuration = 0;
    float currentLevel = 0.0;
    bool active = false;

    void reset() {
        for (int i = 0; i < MAX_STAGES; i++) {
            stages[i] = {0, 0, 0.5};  // Default curveParam to 0.5 for linear
        }
    }

    float computeNextValue(float current, float target, float fraction, float curveParam) {
    // Ensure curveParam is between 0 and 1
    curveParam = constrain(curveParam, 0.0f, 1.0f);
    
    float delta = target - current;

    if (curveParam < 0.333f) {
        // Logarithmic interpolation
        // Map curveParam from [0, 0.333] to [0, 1] for this segment
        float adjustedParam = curveParam * 3;
        float scale = log10(1 + 9 * adjustedParam);
        return current + delta * (log10(1 + 9 * fraction * scale) / scale);
    } else if (curveParam < 0.666f) {
        // Linear interpolation
        // Map curveParam from [0.333, 0.666] to [0, 1] for this segment
        return current + delta * fraction;
    } else {
        // Exponential interpolation
        // Map curveParam from [0.666, 1] to [0, 1] for this segment
        float adjustedParam = (curveParam - 0.666f) * 3;
        return current + delta * (pow(fraction, 2 + 8 * adjustedParam));
    }
}
};

void AudioEffectDynamicEnvelope::update(void) {
    audio_block_t *block;
    block = receiveWritable();
    if (!block) return;

    if (active) {
        for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
            if (currentStage < numStages) {
                float targetLevel = stages[currentStage].targetLevel;
                float delta = 1.0 / (stages[currentStage].duration * (AUDIO_SAMPLE_RATE_EXACT / 1000.0));

                currentLevel = computeNextValue(currentLevel, targetLevel, delta, stages[currentStage].curveParam);

                block->data[i] = (int16_t)(block->data[i] * currentLevel);

                currentDuration += (1000.0 / AUDIO_SAMPLE_RATE_EXACT);
                if (currentDuration >= stages[currentStage].duration) {
                    currentDuration = 0;
                    currentStage++;
                    if (currentStage < numStages) {
                        currentLevel = stages[currentStage - 1].targetLevel;
                    }
                }
            } else {
                block->data[i] = 0;
            }
        }
    }

    transmit(block);
    release(block);
}
