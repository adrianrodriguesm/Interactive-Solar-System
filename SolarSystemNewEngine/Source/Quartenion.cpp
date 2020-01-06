#include "../Include/Quartenion.h"

	
qtrn::qtrn() {
	this->t = 0.0f;
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
}

qtrn::qtrn(float t, float x, float y, float z) {
	this->t = t;
	this->x = x;
	this->y = y;
	this->z = z;
}

const qtrn qtrn::qFromAngleAxis(const float& theta, const vec4& axis)
{
	vec4 axisn = normalize(axis);

	qtrn q;
	float a = theta * (float)DEGREES_TO_RADIANS;
	q.t = cos(a / 2.0f);
	float s = sin(a / 2.0f);
	q.x = axisn.x * s;
	q.y = axisn.y * s;
	q.z = axisn.z * s;

	qClean(q);
	return qNormalize(q);
}
const void qtrn::qToAngleAxis(const qtrn& q,  float& theta, vec4& axis)
{
	qtrn qn = qNormalize(q);
	theta = 2.0f * acos(qn.t) * (float)RADIANS_TO_DEGREES;
	float s = sqrt(1.0f - qn.t * qn.t);
	if (s < qThreshold) {
		axis.x = 1.0f;
		axis.y = 0.0f;
		axis.z = 0.0f;
		axis.w = 1.0f;
	}
	else {
		float sinv = 1 / s;
		axis.x = qn.x * sinv;
		axis.y = qn.y * sinv;
		axis.z = qn.z * sinv;
		axis.w = 1.0f;
	}
}

const void qClean(qtrn& q)
{
	if (fabs(q.t) < qThreshold) q.t = 0.0f;
	if (fabs(q.x) < qThreshold) q.x = 0.0f;
	if (fabs(q.y) < qThreshold) q.y = 0.0f;
	if (fabs(q.z) < qThreshold) q.z = 0.0f;
}

const float qQuadrance(const qtrn& q)
{
	return q.t * q.t + q.x * q.x + q.y * q.y + q.z * q.z;
}

const float qNorm(const qtrn& q)
{
	return sqrt(qQuadrance(q));
}

const qtrn qNormalize(const qtrn& q)
{
	float s = 1 / qNorm(q);
	return qMultiply(q, s);
}

const qtrn qConjugate(const qtrn& q)
{
	qtrn qconj = { q.t, -q.x, -q.y, -q.z };
	return qconj;
}

const qtrn qInverse(const qtrn& q)
{
	return qMultiply(qConjugate(q), 1.0f / qQuadrance(q));
}

const qtrn qAdd(const qtrn& q0, const qtrn& q1)
{
	qtrn q;
	q.t = q0.t + q1.t;
	q.x = q0.x + q1.x;
	q.y = q0.y + q1.y;
	q.z = q0.z + q1.z;
	return q;
}

const qtrn qMultiply(const qtrn& q, const float s)
{
	qtrn sq;
	sq.t = s * q.t;
	sq.x = s * q.x;
	sq.y = s * q.y;
	sq.z = s * q.z;
	return sq;
}

const qtrn qMultiply(const qtrn& q0, const qtrn& q1)
{
	qtrn q;
	q.t = q0.t * q1.t - q0.x * q1.x - q0.y * q1.y - q0.z * q1.z;
	q.x = q0.t * q1.x + q0.x * q1.t + q0.y * q1.z - q0.z * q1.y;
	q.y = q0.t * q1.y + q0.y * q1.t + q0.z * q1.x - q0.x * q1.z;
	q.z = q0.t * q1.z + q0.z * q1.t + q0.x * q1.y - q0.y * q1.x;
	return q;
}

const void qGLMatrix(const qtrn& q, mat4& matrix)
{
	qtrn qn = qNormalize(q);

	float xx = qn.x * qn.x;
	float xy = qn.x * qn.y;
	float xz = qn.x * qn.z;
	float xt = qn.x * qn.t;
	float yy = qn.y * qn.y;
	float yz = qn.y * qn.z;
	float yt = qn.y * qn.t;
	float zz = qn.z * qn.z;
	float zt = qn.z * qn.t;

	matrix.elements[0] = 1.0f - 2.0f * (yy + zz);
	matrix.elements[1] = 2.0f * (xy + zt);
	matrix.elements[2] = 2.0f * (xz - yt);
	matrix.elements[3] = 0.0f;

	matrix.elements[4] = 2.0f * (xy - zt);
	matrix.elements[5] = 1.0f - 2.0f * (xx + zz);
	matrix.elements[6] = 2.0f * (yz + xt);
	matrix.elements[7] = 0.0f;

	matrix.elements[8] = 2.0f * (xz + yt);
	matrix.elements[9] = 2.0f * (yz - xt);
	matrix.elements[10] = 1.0f - 2.0f * (xx + yy);
	matrix.elements[11] = 0.0f;

	matrix.elements[12] = 0.0f;
	matrix.elements[13] = 0.0f;
	matrix.elements[14] = 0.0f;
	matrix.elements[15] = 1.0f;

	mClean(matrix);
}

const qtrn qLerp(const qtrn& q0, const qtrn& q1, float k)
{
	float cos_angle = q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.t * q1.t;
	float k0 = 1.0f - k;
	float k1 = (cos_angle > 0) ? k : -k;
	qtrn qi = qAdd(qMultiply(q0, k0), qMultiply(q1, k1));
	return qNormalize(qi);
}

const qtrn qSlerp(const qtrn& q0, const qtrn& q1, float k)
{
	float angle = acos(q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.t * q1.t);
	float k0 = sin((1 - k) * angle) / sin(angle);
	float k1 = sin(k * angle) / sin(angle);
	qtrn qi = qAdd(qMultiply(q0, k0), qMultiply(q1, k1));
	return qNormalize(qi);
}

const bool qEqual(const qtrn& q0, const qtrn& q1)
{
	return (fabs(q0.t - q1.t) < qThreshold && fabs(q0.x - q1.x) < qThreshold &&
		fabs(q0.y - q1.y) < qThreshold && fabs(q0.z - q1.z) < qThreshold);
}

const void qPrint(const std::string& s, const qtrn& q)
{
	std::cout << s << " = (" << q.t << ", " << q.x << ", " << q.y << ", " << q.z << ")" << std::endl;
}

const void qPrintAngleAxis(const std::string& s, const qtrn& q)
{
	std::cout << s << " = [" << std::endl;

	float thetaf;
	vec4 axis_f;
	qtrn::qToAngleAxis(q, thetaf, axis_f);
	std::cout << "  angle = " << thetaf << std::endl;
	std::cout << axis_f << std::endl;
	std::cout << "]" << std::endl;
}

qtrn& qtrn::operator = (const qtrn& other)
{
	t = other.t;
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}

const qtrn operator + (const qtrn& left, const qtrn& right) {
	return qAdd(left, right);
}
const qtrn operator * (const qtrn& q, const float s) {
	return qMultiply(q, s);
}

const qtrn operator * (const qtrn& q0, const qtrn& q1) {
	return qMultiply(q0, q1);
}

const bool operator == (const qtrn& left, const qtrn& right) {
	return qEqual(left,right);
}

const bool operator != (const qtrn& left, const qtrn& right) {
	return !qEqual(left, right);
}

	
std::ostream& operator<<(std::ostream& stream, const qtrn& q)
{
	stream << "Quartenion : (" << q.t << ", " << q.x << ", " << q.y << ", " << q.z << ")";
	return stream;
}
