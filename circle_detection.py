import cv2


def detect_circle_get_radius(frame):
    # Convert the frame to grayscale
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    blur = cv2.medianBlur(gray, 11)
    thresh = cv2.threshold(blur, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)[1]

    # Morph open
    kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (5, 5))
    opening = cv2.morphologyEx(thresh, cv2.MORPH_OPEN, kernel, iterations=3)

    # Find contours and filter using contour area and aspect ratio
    cnts = cv2.findContours(opening, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    cnts = cnts[0] if len(cnts) == 2 else cnts[1]

    radius = 0
    for c in cnts:
        peri = cv2.arcLength(c, True)
        approx = cv2.approxPolyDP(c, 0.04 * peri, True)
        area = cv2.contourArea(c)
        if len(approx) > 5 and 1000 < area < 500000:
            ((x, y), r) = cv2.minEnclosingCircle(c)
            cv2.circle(frame, (int(x), int(y)), int(r), (36, 255, 12), 2)
            if r > radius:
                radius = r
    # cv2.imshow('thresh', thresh)
    # cv2.imshow('opening', opening)
    # cv2.imshow('image', frame)

    return frame, radius


if __name__ == '__main__':

    cap = cv2.VideoCapture(0)
    # cap = cv2.VideoCapture("data/chota.avi")
    # cap = cv2.VideoCapture("data/gabla.avi")
    # cap = cv2.VideoCapture("data/vada.avi")
    while cap.isOpened():
        # Read a frame from the video
        ret, frame = cap.read()

        if not ret:
            continue

        # Process the frame
        processed_frame, radius = detect_circle_get_radius(frame)
        print(radius)
        # Display the resulting frame
        cv2.imshow('Tomato Detection', processed_frame)
        # Break the loop when 'q' key is pressed
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    cap.release()
    cv2.destroyAllWindows()
