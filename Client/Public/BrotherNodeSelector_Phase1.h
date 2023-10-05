#pragma once
#include "Client_Define.h"
#include "Selector.h"

BEGIN(Client)

class CBrotherNodeSelector_Phase1 final : public CSelector
{
private:
	explicit CBrotherNodeSelector_Phase1() DEFAULT;
	virtual ~CBrotherNodeSelector_Phase1() DEFAULT;

public:
	virtual HRESULT									Initialize(shared_ptr<class CBlackBoard>);

public:
	virtual void									Activate() override;
	virtual STATUS									Invoke(_float fTimeDelta) override;
	virtual void									Terminate() override;

private:
	const BROTHER_PHASE*							m_pPhase	= nullptr;

public:
	static shared_ptr<CBrotherNodeSelector_Phase1>	Create(shared_ptr<class CBlackBoard>);
};

END

/*
Phase 1
������ 1

Mithrix starts the battle with basic attacks, running towards the player to get into melee range.
�̽������� �⺻ �������� ������ �����Ͽ�, �÷��̾�� ���� ������ �����ϱ� ���� �޸���.

Primary: Hammer Smash/Needle Barrage
�� ����: �ظ� ���Ž�/�ϵ� �ٶ���

This skill has two modes depending on whether Mithrix is running or not.
�� ��ų�� �̽������� �޸��� �ִ��� ���ο� ���� �� ���� ��尡 �ִ�.

If Mithrix is not running, he can use Hammer Smash, where he stops and winds up to smash the ground with his hammer.
�̽������� �޸��� ���� ������, �״� �ظӷ� ���� ����ġ�� ���� ���߰� �ֵθ� �� �ִ�.

A direct hit from the hammer's path deals 1200% with a proc coefficient of 1.0.
�ظ��� ���� Ÿ���� 1.0�� ���� ����� 1200%�� ���ظ� �ش�.

As the hammer hits the ground, it creates an explosion also dealing 1200% damage with a proc coefficient of 1.0 in a radius of 12m with linear falloff.
�ظӰ� ���� ������, ���� ���Ҹ� ���� 12m �ݰ濡�� ���� ��� 1.0���� 1200%�� ���ظ� �ִ� ������ ����Ų��.

The combination of both sources of damage can kill the player, but if the first triggers one-shot protection, the blast will not damage the player as it occurs in less than 0.1s from when OSP has been triggered.
�� ���ؿ��� ������ �÷��̾ ���� �� ������, ù ��°�� ���� ��ȣ�� �ߵ���Ű��, OSP�� �ߵ��� ���� 0.1�� �̸��� �ð� ���� ������ �÷��̾�� ���ظ� ���� �ʴ´�.

The attack lasts for 4 seconds, during which Mithrix is open to attacks.
�� ������ 4�� ���� ���ӵǸ�, �̽������� �� �Ⱓ ���� ������ ���� �� �ִ�.

However, attack speed affects the animation of the wind up and how quickly the smash will occur from the moment the skill is activated.
�׷��� ���� �ӵ��� ��ų�� Ȱ��ȭ�� �������� ���Žð� �󸶳� ���� �Ͼ���� ������ ��ģ��.

This skill has a cooldown of 4 seconds.
�� ��ų�� ���� ���ð��� 4���̴�.

If Mithrix is sprinting instead, he can fire up to 12 homing needles moving at 120m/s.
���� �̽������� �޸��� �ִٸ�, �״� 120m/s�� �����̴� �ִ� 12���� ���� �ٴ��� �߻��� �� �ִ�.

Each needle explodes upon impact dealing 60% damage with a proc coefficient of 1.0 in a radius of 2m with no falloff.
�� �ٴ��� �浹�� 2m�� �ݰ� ������ ���� ���� 1.0�� ���� ����� 60%�� ���ظ� �ָ� �����Ѵ�.

This skill has a cooldown of 6 seconds, which begins after the first needle is fired.
�� ��ų�� ���� ���ð��� 6���̸�, ù ��° �ٴ��� �߻�� �Ŀ� ���۵ȴ�.

Once the skill has come off cooldown, it will replenish all 12 stocks regardless of how many have been used.
��ų�� ���� ���ð��� ������, ���� ������ ������� ��� 12���� �ٴ��� ����ȴ�.

If Mithrix toggles sprinting while the skill is on cooldown so that it switches to Hammer Smash and back, all charges will be replenished immediately.
�̽������� ��ų�� ���� ���ð� ���� �޸��⸦ ��ȯ�Ͽ� �ظ� ���Ž÷� �ٲٰ� �ٽ� �ǵ�����, ��� ������ ��� ����ȴ�.

Secondary: Hammer Swing
���� ����: �ظ� ����

Mithrix swipes with his hammer, dealing 200% damage with a proc coefficient of 1.0 while pushing the target away.
�̽������� �ظӷ� �ֵθ���, 1.0�� ���� ����� 200%�� ���ظ� �ְ� ����� �о��.

This attack moves him forwards a moderate amount.
�� ������ �׸� ������ �Ÿ���ŭ ������ �̵���Ų��.

This skill has a cooldown of 5 seconds and the attack animation lasts for 4 seconds which scales with attack speed.
�� ��ų�� ���� ���ð��� 5���̰�, ���� �ִϸ��̼��� 4�� ���� ���ӵǸ� �̴� ���� �ӵ��� ���� �����ȴ�.

Utility: Dash
��ƿ��Ƽ: ���

Mithrix dashes a short distance forward, backward, left, or right for 0.3 seconds.
�̽������� 0.3�� ���� ��, ��, ����, �Ǵ� ���������� ª�� �Ÿ��� ����Ѵ�.

This also removes all his timed debuffs, but not any damage-over-time effects.
�̷ν� ���� ��� �ð��� ������� ���ŵ�����, ���� ���� ȿ���� ���ŵ��� �ʴ´�.

This skill has a cooldown of 3 seconds and holds 2 charges.
�� ��ų�� ���� ���ð��� 3���̸�, 2ȸ�� ������ �����Ѵ�.

Special: Shock Wave
Ư��: ��ũ ���̺�

Mithrix bends down to jump high into the air, which he does after 1 second (scales with attack speed).
�̽������� 1�� �Ŀ�(���� �ӵ��� ���� ����) ���� �ٱ� ���� ���� ���δ�.

After 3 seconds he slams down in the center of the arena, creating a ring-shaped shockwave consisting of 12 waves that travel outwards at 60m/s for 3 seconds.
3�� �Ŀ� �״� �Ʒ��� �߾ӿ� �����Ͽ�, 3�� ���� 60m/s�� �ٱ������� �̵��ϴ� 12���� �ĵ����� ������ ���� ����� ��ũ���̺긦 �����Ѵ�.

The shock wave effectively ends when it reaches the thin line of rocks between the platform where he is first seen.
��ũ���̺�� �װ� ó�� �� �� �ִ� �÷��� ������ ���� ���� �ٿ� �����ϸ� ȿ�������� ������.

Each wave deals 400% with a proc coefficient of 1.0 and inflicts Status Cripple(Armor by 20, Speed by 50%).
�� �ĵ��� 1.0�� ���� ����� 400%�� ���ظ� �ְ� ���� ����(�Ƹ� 20, �̵��ӵ� 50%)�� �����Ѵ�.
 
This skill has a cooldown of 30 seconds.
�� ��ų�� ���� ���ð��� 30���̴�.

Behaviors
�ൿ

Use special (Shock Wave): Mithrix must have less than 75% health.
Ư�� ��� (��ũ ���̺�): �̽������� ü���� 75% �̸��̾�� �Ѵ�.

Use utility and strafe: Mithrix must be within 40m from its target, This behavior cannot be selected twice in a row.
��ƿ��Ƽ�� ���̵� ���� ���: �̽������� ���� 40m �̳��� �־�� �ϸ�, �� �ൿ�� �������� �� �� ���õ� �� ����.

If this behavior is selected as Mithrix is approaching the player, it will very likely be followed up by either his primary or secondary skill.
�� �ൿ�� �̽������� �÷��̾�� �����ϴ� ���߿� ���õǸ�, ���� �� ���� �Ǵ� ���� �������� �̾��� ���ɼ��� �ſ� ����.

This can look like strafing out of the way of a possible incoming attack and then attacking from the flank.
�̰��� ������ ������ ������ ���ϱ� ���� ������ �����̰�, �� ������ ���鿡�� �����ϴ� ��ó�� ���� �� �ִ�.

Use utility: Mithrix must be within 40-50m from its target. This behavior cannot be selected twice in a row.
��ƿ��Ƽ ���: �̽������� ���� 40-50m �̳��� �־�� �ϸ�, �� �ൿ�� �������� �� �� ���õ� �� ����.

Use secondary: Mithrix must be within 40m from its target.
���� ���� ���: �̽������� ���� 40m �̳��� �־�� �Ѵ�.

Use primary (Hammer Smash): Mithrix must be within 35m from its target.
�� ���� ��� (�ظ� ���Ž�): �̽������� ���� 35m �̳��� �־�� �Ѵ�.

Sprint and use primary (Needle Barrage): Mithrix must be more than 70m away from its target, have line of sight, and have less than 80% health.
�޸��� �� ���� ��� (�ϵ� �ٶ���): �̽������� ���� 70m �̻� ������ �־�� �ϸ�, �þ߰� �־�� �ϰ�, ü���� 80% �̸��̾�� �Ѵ�.

Mithrix will ignore the node graph and run directly towards the target. This behavior is active for 2 seconds.
�̽������� ��� �׷����� �����ϰ� ����� ���� ���� �޸���. �� �ൿ�� 2�� ���� Ȱ��ȭ�ȴ�.

Chase off nodegraph: Mithix must be within 50m from its target and have line of sight.
��� �׷����� ��� �߰�: �̽������� ���� 50m �̳��� �־�� �ϸ�, �þ߰� �־�� �Ѵ�.

This behavior is active for 2 seconds and will most likely be selected when Mithrix is near the target after both the primary and secondary are on cooldown.
�� �ൿ�� 2�� ���� Ȱ��ȭ�Ǹ�, �� ���ݰ� ���� ���� ��� ��ٿ� ���� �� �̽������� ��� ������ ���� ���ɼ��� ���� ���õ� ���̴�.

Sprint off nodegraph: Mithrix must be more than 50m away from its target. This behavior is active for 1 second.
��� �׷����� ��� �޸���: �̽������� ���� 50m �̻� ������ �־�� �Ѵ�. �� �ൿ�� 1�� ���� Ȱ��ȭ�ȴ�.

Chase: no requirements.
�߰�: �䱸���� ����.
*/
