# MINIRT 프로젝트 소개

## 프로젝트명: MINIRT

MINIRT는 raycasting 기술을 활용하여 실시간 ray tracing을 구현한 3D 그래픽 프로젝트입니다. 본 프로젝트는 빛의 경로를 추적하여 3D 환경을 시각화하는 방식을 채택하고 있으며, 카메라에서 발사된 광선(ray)이 3D 물체와 교차하는 지점을 계산하여 해당 지점에 적절한 색상을 입힘으로써 현실감 넘치는 3D 이미지를 생성합니다.

이 기술은 광원, 물체의 재질, 그림자, 반사 및 굴절 등의 물리적 현상을 정밀하게 계산하여 실제와 같은 시각적 효과를 제공합니다. MINIRT 프로젝트의 목표는 사용자가 정의한 3D 환경에서 실시간으로 높은 품질의 렌더링을 달성하는 것입니다.

## 주요 특징

- **Raycasting 기반 Ray Tracing:** 카메라에서 발사된 광선이 물체와 교차하는 지점을 계산하여 실사와 같은 3D 이미지를 생성합니다.
- **현실적인 3D 시각화:** 광원의 효과, 물체의 재질, 그림자, 반사 및 굴절 등을 정밀하게 계산하여 실감 나는 이미지를 제공합니다.
- **고성능 렌더링:** 최적화된 알고리즘을 통해 실시간으로 고품질의 3D 렌더링을 구현합니다.

## 산출물
![](https://file.notion.so/f/f/4b5716b5-80e6-4641-a1da-9729209ad23d/ce4cda84-78bd-483b-ac08-521de6f111af/Untitled.png?id=852adde0-d9a0-494e-b833-82d80ceb7509&table=block&spaceId=4b5716b5-80e6-4641-a1da-9729209ad23d&expirationTimestamp=1709971200000&signature=8FrJk5YfBcL76kH8ZXEX-a6wQd-zxG2h5uK1_aNRiRY&downloadName=Untitled.png)


![](https://file.notion.so/f/f/4b5716b5-80e6-4641-a1da-9729209ad23d/c47a8a50-0a64-4586-83f4-4c1bc6f5329e/Untitled.png?id=ebf78a0f-4635-4813-b7b3-c040678d4192&table=block&spaceId=4b5716b5-80e6-4641-a1da-9729209ad23d&expirationTimestamp=1709971200000&signature=1kMHpOXV3Z6E3OzBvwbLX2oeHxYR9kWKtGXwoGfL2Ic&downloadName=Untitled.png)


MINIRT 프로젝트를 통해 생성되는 주요 산출물은 사용자가 정의한 3D 환경에서 실시간으로 렌더링된 고품질의 실사 3D 이미지입니다. 이러한 이미지는 게임 개발, 시뮬레이션, 시각 효과 제작 등 다양한 분야에서 응용될 수 있습니다.

## 프로젝트 진행 과정

프로젝트의 전체 진행 과정은 [여기](https://playful-bicycle-9aa.notion.site/MINIRT-f4f84b77f533435191f926b5324f83cf?pvs=4)에서 확인할 수 있습니다. 이 링크를 통해 개발 과정, 프로젝트 업데이트, 팀원들의 공동 작업 내용 등을 상세히 파악할 수 있습니다.



## 기대 효과

MINIRT 프로젝트는 3D 그래픽 개발자들에게 실시간으로 고품질의 렌더링을 구현할 수 있는 강력한 도구를 제공합니다. 또한, 이 기술은 교육, 엔터테인먼트, 건축 시각화 등 여러 분야에서 혁신적인 시각적 경험을 창출할 잠재력을 가지고 있습니다.

# MINIRT 설치 가이드

이 가이드는 MINIRT 프로젝트를 macOS 운영 체제에서 컴파일하고 실행하기 위한 단계를 설명합니다. 아래의 명령어들은 프로젝트의 루트 디렉토리에서 실행되어야 합니다.

## 설치 전 요구 사항

- **macOS 운영 체제가 설치된 시스템**
- **개발 도구 설치**:
    - Xcode Command Line Tools는 필수적입니다. 다음 명령어를 통해 설치할 수 있습니다:
    ```sh
    xcode-select --install
    ```

## 설치 방법

### 1. 라이브러리 컴파일:

MINIRT는 `libft`와 `mlx` 라이브러리에 의존합니다. 이 라이브러리들을 먼저 컴파일해야 합니다.

- `libft`와 `mlx` 라이브러리 컴파일:
    ```sh
    make -C libft all
    make -C mlx all
    ```

### 2. MINIRT 컴파일:

- 기본적으로, `make` 명령어는 필수 부분만 컴파일합니다.
    ```sh
    make
    ```
- 보너스 기능을 포함시키려면 `make bonus` 명령어를 사용합니다.
    ```sh
    make bonus
    ```

### 3. 실행

- 컴파일이 완료된 후, 프로젝트를 실행할 준비가 완료됩니다. 실행 파일은 프로젝트 루트에 생성됩니다.

## 청소 명령어

- 모든 생성된 오브젝트 파일과 실행 파일을 제거하려면, 다음 명령어들을 사용할 수 있습니다:
    - 오브젝트 파일 제거:
        ```sh
        make clean
        ```
    - 오브젝트 파일과 실행 파일 모두 제거:
        ```sh
        make fclean
        ```
    - 모든 것을 제거한 후 다시 컴파일:
        ```sh
        make re
        ```

이 지침을 따르면, MINIRT 프로젝트를 성공적으로 설치하고 실행할 수 있습니다. 추가 도움이 필요한 경우, 프로젝트 문서나 커뮤니티 포럼을 참조하세요.

