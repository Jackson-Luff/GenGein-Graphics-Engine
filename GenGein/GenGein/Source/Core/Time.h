#pragma once

class Time
{
public:

	enum DeltaTimeID
	{
		DT_FIXED,
		DT_DYNAMIC,
		DT_SEMI_FIXED,
		DT_BETTER,
		DT_BEST
	};

	Time();
	Time(const DeltaTimeID a_DtID);

	virtual void Update();

	inline const double& GetDeltaTime() {
		return m_deltaTime;
	}
	inline const double& GetElapsedTime() {
		return m_elapsedTime;
	}

	inline const DeltaTimeID& GetDTType() {
		return m_deltaTimeType;
	}
private:

	void VariedDeltaTime();
	void SemiFixedDeltaTime();
	void BetterDeltaTime();
	void BestDeltaTime();

	DeltaTimeID m_deltaTimeType;

	double m_deltaTime;
	double m_prevTime;

	double m_elapsedTime;
};
