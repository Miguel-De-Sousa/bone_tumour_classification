#include "bonetumourclassifier.h"
#include <cmath>

BoneTumourClassifier::BoneTumourClassifier() : env(ORT_LOGGING_LEVEL_WARNING, "BoneTumourClassifier") {}
BoneTumourClassifier::~BoneTumourClassifier() {}

bool BoneTumourClassifier::loadModel(const QString& modelPath){
    #ifdef _WIN32 //Check for path on Windows platforms
        std::wstring wPath = modelPath.toStdWString();
        const wchar_t* path = wPath.c_str();
    #else
        std::string sPath = modelPath.toStdString();
        const char* path = sPath.c_str();
    #endif

    try {
        Ort::SessionOptions sessionOptions;
        sessionOptions.SetIntraOpNumThreads(1);
        session = std::make_unique<Ort::Session>(env, path, sessionOptions);
        return true;
    } 
    catch (...) {
        return false;
    }
}

ModelResult BoneTumourClassifier::predict(const QString& imagePath) {
    ModelResult result { false, 0.0f, "Error running" };
    if (!session) return result;

    QImage img(imagePath);
    if (img.isNull()) return result;

    QImage scaledImg = img.scaled(224, 224, Qt::IgnoreAspectRatio).convertToFormat(QImage::Format_RGB888);
    std::vector<float> inputTensorValues(1 * 3 * 224 * 224);

    int channelStride = 224 * 224;
    for (int y = 0; y < 224; ++y) {
        for (int x = 0; x < 224; ++x) {
            QRgb pixel = scaledImg.pixel(x, y);
            float r = qRed(pixel) / 255.0f;
            float g = qGreen(pixel) / 255.0f;
            float b = qBlue(pixel) / 255.0f;

            int pixelOffset = (y * 224) + x;
            inputTensorValues[0 * channelStride + pixelOffset] = r;
            inputTensorValues[1 * channelStride + pixelOffset] = g;
            inputTensorValues[2 * channelStride + pixelOffset] = b;
        }
    }

    std::array<int64_t, 4> inputShape = {1, 3, 224, 224};
    Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
    Ort::Value inputTensor = Ort::Value::CreateTensor<float>(memoryInfo, inputTensorValues.data(), inputTensorValues.size(), inputShape.data(), inputShape.size());

    const char* inputNames[] = {"images"};
    const char* outputNames[] = {"output0"};

    try {
        auto outputTensors = session->Run(Ort::RunOptions{nullptr}, inputNames, &inputTensor, 1, outputNames, 1);
        float* rawOutputScores = outputTensors.front().GetTensorMutableData<float>();

        float exp_b = std::exp(rawOutputScores[0]);
        float exp_m = std::exp(rawOutputScores[1]);
        float sum = exp_b + exp_m;

        float benignPercentage = (exp_b / sum) * 100.0f;
        float malignantPercentage = (exp_m / sum) * 100.0f;

        if (malignantPercentage > 50.0f) {
            result.isMalignant = true;
            result.confidence = malignantPercentage;
            result.displayString = QString("Prediction: MALIGNANT\nConfidence: %1%").arg(malignantPercentage, 0, 'f', 1);
        } else {
            result.isMalignant = false;
            result.confidence = benignPercentage;
            result.displayString = QString("Prediction: BENIGN\nConfidence: %1%").arg(benignPercentage, 0, 'f', 1);
        }
    }
    catch (...) {}

    return result;
}