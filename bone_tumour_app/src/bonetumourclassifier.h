#include <QString>
#include <QImage>
#include <memory>
#include <onnxruntime_cxx_api.h>

struct ModelResult 
{
    bool isMalignant;
    float confidence;
    QString displayString;
};

class BoneTumourClassifier
{
public:
    BoneTumourClassifier();
    ~BoneTumourClassifier();

    bool loadModel(const QString& modelPath);
    ModelResult predict(const QString& imagePath);

private:
    //Create onnx runtime environment
    Ort::Env env;
    std::unique_ptr<Ort::Session> session;
};