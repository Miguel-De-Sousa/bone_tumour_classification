import os
from ultralytics import YOLO
    
model = YOLO('yolov8s.pt')

model.train(
    data = 'data.yaml',
    epochs = 100,
    imgsz=1024,
    batch=8,
    device=0,
    project = '/kaggke/working/runs'
)
    