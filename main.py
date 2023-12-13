import cv2
import serial
from ultralytics import YOLO
from circle_detection import detect_circle_get_radius


def get_radius_quality(model, n_samples):
    cap = cv2.VideoCapture(0)
    count_samples = 0
    sum_radius = 0.0
    sum_confidence = 0.0
    final_radius = 0.0
    final_confidence = 0.0
    while cap.isOpened():
        # Read a frame from the video
        ret, frame = cap.read()

        if not ret:
            continue

        # Process the frame
        processed_frame, radius = detect_circle_get_radius(frame)
        results = model(frame, show = True, max_det = 1)
        # Display the resulting frame
        # cv2.imshow('Tomato Detection', processed_frame)
        count_samples += 1
        sum_radius += radius
        # if results[0].boxes is not NULL:
        sum_confidence += float(results[0].boxes.conf[0])
        if count_samples == n_samples:
            final_radius = sum_radius / n_samples
            final_confidence = sum_confidence / n_samples
            break
        # Break the loop when 'q' key is pressed
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()
    return final_radius, final_confidence
    # Release the video capture object and close the OpenCV window


def main():
    model = YOLO("YOLO/best.pt")
    print("YOLO model loaded")
    # ser = serial.Serial(
    #     'COM3', 9600
    # )
    print("Serially connected")
    while True:

        # s = ser.readline().decode("utf-8")
        # s = s[:s.find('\r')]
        # print("Input from arduino ", s)

        radius, confidence = get_radius_quality(model, n_samples=50)
        print(f"Radius of tomato: {radius:0.2f} px")
        print(f"Rotten Tomatoes confidence: {confidence:0.2f} ")
        if confidence > 0.95:
            print("Rotten tomatoes detected")
            # ser.write("-1".encode("utf-8"))
        else:
            if radius > 120 and radius < 135 :
                print("Best tomatoes detected")
                # ser.write("1".encode("utf-8"))
            else:
                print("Good tomatoes detected")
                # ser.write("2".encode("utf-8"))


if __name__ == '__main__':
    # chota 100
    # gabla 125
    # vada 142
    main()
