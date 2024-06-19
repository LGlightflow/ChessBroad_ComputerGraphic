import cv2

def bmp_to_bgr(bmp_path):
    # 读取BMP图片
    image = cv2.imread(bmp_path, cv2.IMREAD_UNCHANGED)

    # 如果图片为4通道（包含alpha通道），去掉alpha通道
    gbr_image = image[:, :, [1, 2, 0]]
    cv2.imwrite(r"D:\project\program\vs2017project\ConsoleApplication1\tex\12.bmp",image)
    return image

# 替换为你的BMP图片路径
bmp_path = r"D:\project\program\vs2017project\ConsoleApplication1\tex\sky.bmp"

# 转换为BGR格式
bgr_image = bmp_to_bgr(bmp_path)

# 显示BGR图片
cv2.imshow("BGR Image", bgr_image)
cv2.waitKey(0)
cv2.destroyAllWindows()