#include "Quaternions.h""




qtrn::qtrn() {
	t = x = y = z = 0.f;
}

qtrn::qtrn(float tI, float xI, float yI, float zI) {
	t = tI;
	x = xI;
	y = yI;
	z = zI;
}

qtrn& qtrn::operator=(const qtrn& q) {
	this->t = q.t;
	this->x = q.x;
	this->y = q.y;
	this->z = q.z;
	return *this;
}

const qtrn qtrn::qFromAngleAxis(float theta, vec4 axis)
{
	vec4 axisn = normalize(axis);

	qtrn q;
	float a = theta * (float)DEGREES_TO_RADIANS;
	q.t = cos(a / 2.0f);
	float s = sin(a / 2.0f);
	q.x = axisn.x * s;
	q.y = axisn.y * s;
	q.z = axisn.z * s;

	clean(q);
	return normalize(q);
}

const void qToAngleAxis(const qtrn& q, float& theta, vec4& axis)
{
	qtrn qn = normalize(q);
	theta = 2.0f * acos(qn.t) * (float)RADIANS_TO_DEGREES;
	float s = sqrt(1.0f - qn.t * qn.t);
	if (s < q.threshold) {
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

const void  clean(qtrn& q)
{
	if (fabs(q.t) < q.threshold) q.t = 0.0f;
	if (fabs(q.x) < q.threshold) q.x = 0.0f;
	if (fabs(q.y) < q.threshold) q.y = 0.0f;
	if (fabs(q.z) < q.threshold) q.z = 0.0f;
}

const float  quadrance(const qtrn& q)
{
	return q.t * q.t + q.x * q.x + q.y * q.y + q.z * q.z;
}

const float  norm(const qtrn& q)
{
	return sqrt(quadrance(q));
}

const qtrn normalize(const qtrn& q)
{
	float s = 1 / norm(q);
	return multiply(q, s);
}

const qtrn  conjugate(const qtrn& q)
{
	qtrn qconj = { q.t, -q.x, -q.y, -q.z };
	return qconj;
}

const qtrn  inverse(const qtrn& q)
{
	return multiply(conjugate(q), 1.0f / quadrance(q));
}

const qtrn add(const qtrn& q0, const qtrn& q1) {

	qtrn q;
	q.t = q0.t + q1.t;
	q.x = q0.x + q1.x;
	q.y = q0.y + q1.y;
	q.z = q0.z + q1.z;
	return q;
}

const qtrn operator+(const qtrn& q0, const qtrn& q1) {

	qtrn q;
	q.t = q0.t + q1.t;
	q.x = q0.x + q1.x;
	q.y = q0.y + q1.y;
	q.z = q0.z + q1.z;
	return q;
}

const qtrn operator*(const qtrn& q0, const qtrn& q1) {
	qtrn q;
	q.t = q0.t * q1.t - q0.x * q1.x - q0.y * q1.y - q0.z * q1.z;
	q.x = q0.t * q1.x + q0.x * q1.t + q0.y * q1.z - q0.z * q1.y;
	q.y = q0.t * q1.y + q0.y * q1.t + q0.z * q1.x - q0.x * q1.z;
	q.z = q0.t * q1.z + q0.z * q1.t + q0.x * q1.y - q0.y * q1.x;
	return q;
}

const qtrn operator*(const qtrn& q, const float s) {
	qtrn sq;
	sq.t = s * q.t;
	sq.x = s * q.x;
	sq.y = s * q.y;
	sq.z = s * q.z;
	return sq;
}

const qtrn operator*(const float s, const qtrn& q) {
	qtrn sq;
	sq.t = s * q.t;
	sq.x = s * q.x;
	sq.y = s * q.y;
	sq.z = s * q.z;
	return sq;
}

const qtrn  multiply(const qtrn& q, const float s)
{
	qtrn sq;
	sq.t = s * q.t;
	sq.x = s * q.x;
	sq.y = s * q.y;
	sq.z = s * q.z;
	return sq;
}

const qtrn  multiply(const qtrn& q0, const qtrn& q1)
{
	qtrn q;
	q.t = q0.t * q1.t - q0.x * q1.x - q0.y * q1.y - q0.z * q1.z;
	q.x = q0.t * q1.x + q0.x * q1.t + q0.y * q1.z - q0.z * q1.y;
	q.y = q0.t * q1.y + q0.y * q1.t + q0.z * q1.x - q0.x * q1.z;
	q.z = q0.t * q1.z + q0.z * q1.t + q0.x * q1.y - q0.y * q1.x;
	return q;
}

const void  qGLMatrix(const qtrn& q, mat4& matrix)
{
	qtrn qn = normalize(q);

	float xx = qn.x * qn.x;
	float xy = qn.x * qn.y;
	float xz = qn.x * qn.z;
	float xt = qn.x * qn.t;
	float yy = qn.y * qn.y;
	float yz = qn.y * qn.z;
	float yt = qn.y * qn.t;
	float zz = qn.z * qn.z;
	float zt = qn.z * qn.t;

	matrix.data[0] = 1.0f - 2.0f * (yy + zz);
	matrix.data[1] = 2.0f * (xy + zt);
	matrix.data[2] = 2.0f * (xz - yt);
	matrix.data[3] = 0.0f;

	matrix.data[4] = 2.0f * (xy - zt);
	matrix.data[5] = 1.0f - 2.0f * (xx + zz);
	matrix.data[6] = 2.0f * (yz + xt);
	matrix.data[7] = 0.0f;

	matrix.data[8] = 2.0f * (xz + yt);
	matrix.data[9] = 2.0f * (yz - xt);
	matrix.data[10] = 1.0f - 2.0f * (xx + yy);
	matrix.data[11] = 0.0f;

	matrix.data[12] = 0.0f;
	matrix.data[13] = 0.0f;
	matrix.data[14] = 0.0f;
	matrix.data[15] = 1.0f;
}

const mat4 matrixFromQtrn(const qtrn& q)
{
	qtrn qn = normalize(q);

	float xx = qn.x * qn.x;
	float xy = qn.x * qn.y;
	float xz = qn.x * qn.z;
	float xt = qn.x * qn.t;
	float yy = qn.y * qn.y;
	float yz = qn.y * qn.z;
	float yt = qn.y * qn.t;
	float zz = qn.z * qn.z;
	float zt = qn.z * qn.t;

	mat4 matrix = mat4();

	matrix.data[0] = 1.0f - 2.0f * (yy + zz);
	matrix.data[4] = 2.0f * (xy + zt);
	matrix.data[8] = 2.0f * (xz - yt);
	matrix.data[12] = 0.0f;

	matrix.data[1] = 2.0f * (xy - zt);
	matrix.data[5] = 1.0f - 2.0f * (xx + zz);
	matrix.data[9] = 2.0f * (yz + xt);
	matrix.data[13] = 0.0f;

	matrix.data[2] = 2.0f * (xz + yt);
	matrix.data[6] = 2.0f * (yz - xt);
	matrix.data[10] = 1.0f - 2.0f * (xx + yy);
	matrix.data[14] = 0.0f;

	matrix.data[3] = 0.0f;
	matrix.data[7] = 0.0f;
	matrix.data[11] = 0.0f;
	matrix.data[15] = 1.0f;

	return matrix;
}

const qtrn  qLerp(const qtrn& q0, const qtrn& q1, float k)
{
	float cos_angle = q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.t * q1.t;
	float k0 = 1.0f - k;
	float k1 = (cos_angle > 0) ? k : -k;
	qtrn qi = add(multiply(q0, k0), multiply(q1, k1));
	return normalize(qi);
}

const qtrn  qSlerp(const qtrn& q0, const qtrn& q1, float k)
{
	float angle = acos(q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.t * q1.t);
	float k0 = sin((1 - k) * angle) / sin(angle);
	float k1 = sin(k * angle) / sin(angle);
	qtrn qi = add(multiply(q0, k0), multiply(q1, k1));
	return normalize(qi);
}

const bool  qEqual(const qtrn& q0, const qtrn& q1)
{
	return (fabs(q0.t - q1.t) < q0.threshold && fabs(q0.x - q1.x) < q0.threshold &&
		fabs(q0.y - q1.y) < q0.threshold && fabs(q0.z - q1.z) < q0.threshold);
}

const void  print(const std::string& s, const qtrn& q)
{
	std::cout << s << " = (" << q.t << ", " << q.x << ", " << q.y << ", " << q.z << ")" << std::endl;
}

const void printAngleAxis(const std::string& s, const qtrn& q)
{
	std::cout << s << " = [" << std::endl;

	float thetaf;
	vec4 axis_f;
	qToAngleAxis(q, thetaf, axis_f);
	std::cout << "  angle = " << thetaf << std::endl;
	std::cout << "  axis = " << axis_f;
	std::cout << "]" << std::endl;
}