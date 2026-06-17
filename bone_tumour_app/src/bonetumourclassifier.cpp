#include "bonetumourclassifier.h"
#include <algorithm>
#include <QImage>
#include <QRgb>

// Constructor initializes the core ONNX Runtime Environment
BoneTumourClassifier::BoneTumourClassifier() 
    : env(ORT_LOGGING_LEVEL_WARNING, "BoneTumourClassifier") {}

// Destructor is handled automatically by std::unique_ptr
BoneTumourClassifier::~BoneTumourClassifier() {}

bool BoneTumourClassifier::loadModel(const QString& modelPath) {
    #ifdef _WIN32 // Check for path configuration on Windows platforms
        std::wstring wPath = modelPath.toStdWString();
        const wchar_t* path = wPath.c_str();
    #else // macOS and Linux handling
        std::string sPath = modelPath.toStdString();
        const char* path = sPath.c_str();
    #endif

    try {
        Ort::SessionOptions sessionOptions;
        sessionOptions.SetIntraOpNumThreads(1); // Restrict to a single background thread
    
        session = std::make_unique<Ort::Session>(env, path, sessionOptions);
        return true;
    } 
    catch (...) {
        return false;
    }
}

DetectionResult BoneTumourClassifier::predict(const QString& imagePath) {
    DetectionResult result;
    result.success = false;

    try {
        QImage img(imagePath);
        if (img.isNull()) {
            result.success = false;
            result.errorMessage = "Failed to load image file from disk.";
            return result;
        }

        int modelWidth = 1024;
        int modelHeight = 1024;
        QImage scaledImg = img.scaled(modelWidth, modelHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        // Convert to standard 32-bit RGB format
        scaledImg = scaledImg.convertToFormat(QImage::Format_RGB32);

        std::vector<float> inputTensorValues(1 * 3 * modelWidth * modelHeight);

        // Extract raw pixels and normalise values from 0-255 down to 0.0-1.0
        for (int y = 0; y < modelHeight; ++y) {
            for (int x = 0; x < modelWidth; ++x) {
                QRgb pixel = scaledImg.pixel(x, y);
                
                int r = y * modelWidth + x;
                int g = modelWidth * modelHeight + r;
                int b = 2 * modelWidth * modelHeight + r;

                inputTensorValues[r] = qRed(pixel) / 255.0f;
                inputTensorValues[g] = qGreen(pixel) / 255.0f;
                inputTensorValues[b] = qBlue(pixel) / 255.0f;
            }
        }

        std::vector<int64_t> inputShape = {1, 3, modelWidth, modelHeight};
        Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
        
        Ort::Value inputTensor = Ort::Value::CreateTensor<float>(
            memoryInfo, 
            inputTensorValues.data(), 
            inputTensorValues.size(), 
            inputShape.data(), 
            inputShape.size()
        );

        const char* inputNames[] = {"images"};
        const char* outputNames[] = {"output0"};

        auto outputTensors = session->Run(Ort::RunOptions{nullptr}, inputNames, &inputTensor, 1, outputNames, 1);

        float* outputData = outputTensors[0].GetTensorMutableData<float>();
        auto outputInfo = outputTensors[0].GetTensorTypeAndShapeInfo();
        std::vector<int64_t> outputShape = outputInfo.GetShape(); 

        int numAttributes = outputShape[1];
        int numCandidates = outputShape[2];

        int bestClassId = -1;
        int bestCandidateIndex = -1;
        float highestScore = 0.0f;

        for (int i = 0; i < numCandidates; ++i) {
            for (int classId = 0; classId < 9; ++classId) {
                int dataIndex = (4 + classId) * numCandidates + i;
                float score = outputData[dataIndex];

                if (score > highestScore) {
                    highestScore = score;
                    bestClassId = classId;
                    bestCandidateIndex = i;
                }
            }
        }

        if (bestClassId != -1 && highestScore > 0.25f) {
            result.className = classNames[bestClassId];
            result.confidence = highestScore;

            float cx = outputData[0 * numCandidates + bestCandidateIndex];
            float cy = outputData[1 * numCandidates + bestCandidateIndex];
            float w  = outputData[2 * numCandidates + bestCandidateIndex];
            float h  = outputData[3 * numCandidates + bestCandidateIndex];

            float scaleX = static_cast<float>(img.width()) / modelWidth;
            float scaleY = static_cast<float>(img.height()) / modelHeight;

            cx *= scaleX;
            cy *= scaleY;
            w  *= scaleX;
            h  *= scaleY;

            int topLeftX = static_cast<int>(cx - (w / 2.0f));
            int topLeftY = static_cast<int>(cy - (h / 2.0f));

            result.boundingBox = QRect(topLeftX, topLeftY, static_cast<int>(w), static_cast<int>(h));

            if (result.className == "Osteosarcoma" || result.className == "Other Malignant Tumour") {
                result.severity = "Malignant";
            } else if (result.className == "Giant Cell Tumour") {
                result.severity = "Aggressive Benign";
            } else {
                result.severity = "Benign";
            }
            result.success = true;
        } else {
            result.className = "No Tumour Detected";
            result.severity = "Clear";
            result.confidence = 0.0f;
            result.boundingBox = QRect();
            result.success = true;
        }

    } catch (const std::exception& e) {
        result.success = false;
        result.errorMessage = QString("Model detection failure: %1").arg(e.what());
    }

    return result;
}