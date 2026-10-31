Vector3 GetSilentDirection(Vector3 origin, Vector3 targetPos)
{

    Vector3 delta;
    delta.x = targetPos.x - origin.x;
    delta.y = targetPos.y - origin.y;
    delta.z = targetPos.z - origin.z;

    float length = sqrtf(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);

    if (length > 0.0001f)
    {
        delta.x /= length;
        delta.y /= length;
        delta.z /= length;
    }

    return delta;
}

int isInsideFov(float x, float y, float radius)
{
    float centerX = width / 2.0f;
    float centerY = height / 2.0f;
    float dx = x - centerX;
    float dy = y - centerY;

    return (dx * dx + dy * dy) <= (radius * radius);

}
