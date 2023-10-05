#pragma once
#include "Client_Define.h"
#include "Selector.h"

BEGIN(Engine)
END

BEGIN(Client)

class CBrotherNodeSelector_Phase3 final : public CSelector
{
private:
	explicit CBrotherNodeSelector_Phase3() DEFAULT;
	virtual ~CBrotherNodeSelector_Phase3() DEFAULT;

public:
	virtual HRESULT									Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void									Activate() override;
	virtual STATUS									Invoke(_float fTimeDelta) override;
	virtual void									Terminate() override;

private:
	BROTHER_PHASE*									m_pPhase	= nullptr;

public:
	static shared_ptr<CBrotherNodeSelector_Phase3>	Create(shared_ptr<class CBlackBoard>);
};

END

/*
Phase 3
Mithrix returns with his hammer glowing blue and some of his attacks are now upgraded. Lunar enemies will also continue spawning occasionally.

������ 3
Mithrix�� �Ķ������� ������ ��ġ�� �Բ� ���ƿ��� �Ϻ� ������ ���׷��̵�˴ϴ�. �糪(��) ���鵵 ������ ����ؼ� �����˴ϴ�.

Primary: Lunar Hammer Smash/Needle Barrage
Lunar Hammer Smash�� Needle Barrage�� �� �����Դϴ�.

Mithrix's Hammer Smash is now energized and upon explosion it will create two additional effects.
Mithrix�� Hammer Smash�� ���� �������� �� ���� ���� �� �� ���� �߰� ȿ���� �����մϴ�.
3 small shock waves are sent forward, similar to those from his Shock Wave skill.
���� ���� ����İ� �������� �߻�Ǹ�, �̴� ���� Shock Wave ��ų�� �����մϴ�.
These deal 200% with a proc coefficient of 1.0 and also inflict Cripple.
�̰͵��� 1.0�� proc ����� �Բ� 200%�� �������� �ְ� ���� Cripple�� ���մϴ�.

A lingering pillar of blue flame is also created at the hammer's point of impact, which deals 75% damage with a proc coefficient of 0.2 and ticks 6 times per second for 45 seconds.
���� ��ġ�� ���� ���� �������� �Ķ� ���� ����� ���� 0.2�� proc ����� �Բ� 75%�� �������� ������ 45�� ���� �ʴ� 6���� �������� �ݴϴ�.

These pillars are 40m tall and can go through terrain.
�� ��յ��� 40m ���̰� �ְ� ������ ������ �� �ֽ��ϴ�.
This means that if the player is on top of a ramp on the outer areas of the arena and Mithrix smashes the ground underneath, the pillar can go through the ramp's ceiling and damage the player.
�̴� �÷��̾ �Ʒ��� �ܰ��� ���� ���� �ְ� Mithrix�� �Ʒ����� �������� ���, �� ����� ������ õ���� �����Ͽ� �÷��̾�� �������� �� �� �ִٴ� ���� �ǹ��մϴ�.
These pillars can also be blocked by Defensive Microbots.
�� ��յ��� ���� Defensive Microbots�� ���� ���ܵ� �� �ֽ��ϴ�.

Special: Shock Wave/Big Spinny
Ư�� ����: Shock Wave/Big Spinny

After Mithrix performs his Shock Wave leap, his special skill is replaced by a new one which becomes available after a cooldown of 8 seconds.
Mithrix�� Shock Wave ������ ������ �Ŀ��� ���� Ư�� ��ų�� 8���� ��ٿ� �Ŀ� ��� ������ ���ο� ��ų�� �ٲ�ϴ�.

Once this new skill has been activated, it will revert back to Shock Wave.
�� ���ο� ��ų�� Ȱ��ȭ�Ǹ�, �ٽ� Shock Wave�� �ǵ��ư��ϴ�.

With this new skill Mithrix creates 9 pillars that rotate either clockwise or anticlockwise.
�� ���ο� ��ų�� Mithrix�� �ð� ���� �Ǵ� �ݽð� �������� ȸ���ϴ� 9���� ����� ����ϴ�.
After 3 seconds they deal damage on the spot for 900% damage with a proc coefficient of 1.0 and also inflict Cripple.
3�� �Ŀ� �̵��� 1.0�� proc ����� �Բ� ���忡�� 900%�� �������� �ְ� ���� Cripple�� ���մϴ�.

Mithrix will execute this attack 5 times in a row, with each time the rotation being random.
Mithrix�� �� ������ �������� 5�� ������ ���̸�, �Ź� ȸ�� ������ �����մϴ�.

Behaviors
�ൿ

Mithrix behaves the same away as in Phase 1, but he has one new behavior that supersedes every other in terms of priority for his new special attack.
Mithrix�� ������ 1�� ���������� �ൿ������, ���� ���ο� Ư�� ���ݿ� ���� �켱�������� ��� �ٸ� �ͺ��� �켱�ϴ� ���ο� �ൿ�� �ϳ� �ֽ��ϴ�.

Use special (Big Spinny): Mithrix must have less than 80% health.
Ư�� ���� ��� (Big Spinny): Mithrix�� ü���� 80% �̸��̾�� �մϴ�.

In theory this will always be selected when the skill comes off cooldown after Shock Wave, since the latter requires less than 75% health to be used.
�̷������� �̰��� Shock Wave �Ŀ� ��ų ��ٿ��� ������ �׻� ���õ� ���Դϴ�, ���ڴ� ����ϱ� ���� 75% �̸��� ü���� �ʿ��ϱ� �����Դϴ�.

However, if Mithrix has healing items from the Artifact of Evolution, it is possible to use Shock Wave after having less than 75% health, but then recover to more than 80% before Big Spinny comes off cooldown.
�׷���, Mithrix�� Evolution Artifact of Evolution���� ȸ�� �������� ������ �ִٸ�, 75% �̸��� ü���� ���� ���¿��� Shock Wave�� ����� �Ŀ� Big Spinny�� ��ٿ��� ������ ���� 80% �̻����� ȸ���� �� �ֽ��ϴ�.
*/
