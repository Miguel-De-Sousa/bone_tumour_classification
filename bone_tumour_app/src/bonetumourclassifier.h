#include <QString>
#include <vector>
#include <memory>
#include <onnxruntime_cxx_api.h>

struct DetectionResult {
    QString className;
    float confidence;
    QString severity; 
    bool success;
    QString errorMessage;
};

class BoneTumourClassifier {
public:
    BoneTumourClassifier();
    ~BoneTumourClassifier();

    bool loadModel(const QString& modelPath);
    DetectionResult predict(const QString& imagePath);

private:
    std::unique_ptr<Ort::Session> session;
    Ort::Env env;

    const std::vector<QString> classNames = {
        "Giant Cell Tumor", "Multiple Osteochondromas", "Osteochondroma",
        "Osteofibroma", "Osteosarcoma", "Other Benign Tumor",
        "Other Malignant Tumor", "Simple Bone Cyst", "Synovial Osteochondroma"
    };
};