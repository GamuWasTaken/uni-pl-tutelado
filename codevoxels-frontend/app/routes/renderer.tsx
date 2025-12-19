import { Canvas } from '@react-three/fiber'
import { useEffect, useMemo, useRef, useState } from 'react';
import { OrbitControls, GizmoHelper, GizmoViewport, useGLTF } from '@react-three/drei';
import * as THREE from 'three';

import './renderer.css';
import './global.css';

function VoxelBot() {
  const { scene } = useGLTF('/voxelbot.glb');
  return <primitive object={scene} position={[0.0,-0.5,0.0]} scale={0.6} />;
}

export default function Renderer() {
    const dimensions = {
      x: 10,
      y: 10,
      z: 10
    }

    // Initialize 3D array with RGBA values [R, G, B, A]
    const initializeVoxelWorld = () => {
      const world: number[][][][] = [];
      for (let x = 0; x < dimensions.x; x++) {
        world[x] = [];
        for (let y = 0; y < dimensions.y; y++) {
          world[x][y] = [];
          for (let z = 0; z < dimensions.z; z++) {
            // Initialize each voxel with a default RGBA value [R, G, B, A]
            // Values range from 0-255 for RGB, 0-1 for A
            world[x][y][z] = [0, 0, 0, 0]; // white, transparent
          }
        }
      }
      return world;
    };

    const [voxelWorldState, setVoxelWorldState] = useState<number[][][][]>(initializeVoxelWorld());
    const [isBlockSpacingActive, setIsBlockSpacingActive] = useState(true);

    // Cursor state: position and direction (0=+x, 1=+z, 2=-x, 3=-z)
    const [cursorPos, setCursorPos] = useState({ x: dimensions.x-1, y: dimensions.y-1, z: dimensions.z-1 });
    const [cursorDirection, setCursorDirection] = useState(0);

    const spacingOffset = isBlockSpacingActive ? 0.1 : 0.0;
    const spacingFactor = (1 + spacingOffset);

    // Calculate the center of the voxel cube
    const center: [number, number, number] = [
      ((dimensions.x - 1) * spacingFactor) / 2,
      ((dimensions.y - 1) * spacingFactor) / 2,
      ((dimensions.z - 1) * spacingFactor) / 2
    ];

    // Memoized voxel meshes - only recompute when state or spacing changes
    const voxelMeshes = useMemo(() => {
      const voxels = [];

      for (let x = 0; x < dimensions.x; x++) {
        for (let y = 0; y < dimensions.y; y++) {
          for (let z = 0; z < dimensions.z; z++) {
            const rgba = voxelWorldState[x][y][z];
            const [r, g, b, a] = rgba;

            // Convert RGB from 0-255 to 0-1 for Three.js
            const color: [number, number, number] = [r / 255, g / 255, b / 255];

            // Calculate position with spacing
            const position: [number, number, number] = [
              x * spacingFactor,
              y * spacingFactor,
              z * spacingFactor
            ];

            // Only render voxels with alpha > 0 (visible voxels)
            if (a > 0) {
              voxels.push(
                <mesh key={`voxel-${x}-${y}-${z}`} position={position}>
                  <boxGeometry args={[1, 1, 1]} />
                  <meshLambertMaterial
                    color={color}
                    transparent={a < 1}
                    opacity={a}
                  />
                </mesh>
              );
            }
          }
        }
      }

      return voxels;
    }, [voxelWorldState, spacingFactor]);

    useEffect(() => {
      // Example: Set some colored voxels
      const newWorld = initializeVoxelWorld();

      // Create a simple pattern with different colors
      for (let i = 0; i < 5; i++) {
        newWorld[i][0][0] = [255, 0, 0, 1]; // red
        newWorld[0][i][0] = [0, 255, 0, 1]; // green
        newWorld[0][0][i] = [0, 0, 255, 1]; // blue
      }
      setVoxelWorldState(newWorld);


    }, []);

    // Control functions for the cursor
    const forward = () => {
      setCursorPos(prev => {
        const directions = [
          { x: 1, z: 0 },  // 0: +x
          { x: 0, z: -1 },  // 3: -z
          { x: -1, z: 0 }, // 2: -x
          { x: 0, z: 1 }  // 1: +z
        ];
        const delta = directions[cursorDirection];
        const newX = Math.max(0, Math.min(dimensions.x - 1, prev.x + delta.x));
        const newZ = Math.max(0, Math.min(dimensions.z - 1, prev.z + delta.z));
        return { ...prev, x: newX, z: newZ };
      });
    };

    const turnRight = () => {
      setCursorDirection(prev => (prev - 1 + 4 ) % 4);
    };

    const turnLeft = () => {
      setCursorDirection(prev => (prev + 1 ) % 4);
    };

    const up = () => {
      setCursorPos(prev => ({
        ...prev,
        y: Math.min(dimensions.y - 1, prev.y + 1)
      }));
    };

    const down = () => {
      setCursorPos(prev => ({
        ...prev,
        y: Math.max(0, prev.y - 1)
      }));
    };

    const setBlock = (r: number = 255, g: number = 255, b: number = 255, a: number = 1) => {
      setVoxelWorldState(prev => {
        const newWorld = prev.map(xArr =>
          xArr.map(yArr =>
            yArr.map(rgba => [...rgba])
          )
        );
        newWorld[cursorPos.x][cursorPos.y][cursorPos.z] = [r, g, b, a];
        return newWorld;
      });
    };

    // Expose control functions globally
    useEffect(() => {
      (window as any).forward = forward;
      (window as any).turnRight = turnRight;
      (window as any).turnLeft = turnLeft;
      (window as any).up = up;
      (window as any).down = down;
      (window as any).setBlock = setBlock;

      return () => {
        delete (window as any).forward;
        delete (window as any).turnRight;
        delete (window as any).turnLeft;
        delete (window as any).up;
        delete (window as any).down;
        delete (window as any).setBlock;
      };
    }, [forward, turnRight, turnLeft, up, down, setBlock]);

    const mountRef = useRef<HTMLDivElement>(null);
  return (<>
    <div className="title-card-container">
      <div className="renderer-header">
          <span className="output-title-bar">
            Output
          <div className="block-spacing-toggle">
            <input type="checkbox"
              name="BlockSpacing"
              id="block_spacing"
              checked={isBlockSpacingActive}
              onChange={(event) => setIsBlockSpacingActive(event.target.checked)} />
            <label>
              Block Spacing
            </label>
          </div>
          </span>
      </div>
    <div className="canvas-container">
      <Canvas camera={{ position: [center[0] + 15, center[1] + 15, center[2] + 15], fov: 50 }}>
        {/* Wireframe cube delimiting the world space */}
        <lineSegments position={center}>
          <edgesGeometry args={[
            new THREE.BoxGeometry(
              dimensions.x * spacingFactor,
              dimensions.y * spacingFactor,
              dimensions.z * spacingFactor
            )
          ]} />
          <lineBasicMaterial color="white" />
        </lineSegments>
        {/* Render all voxels from the 3D array */}
        {voxelMeshes}
        {/* Cursor cube */}
        <mesh position={[
          cursorPos.x * spacingFactor,
          cursorPos.y * spacingFactor,
          cursorPos.z * spacingFactor
        ]} rotation={[0, (cursorDirection * Math.PI) / 2, 0]}>
          <VoxelBot />
        </mesh>
        <ambientLight intensity={1.0} />
        <directionalLight position={[0, 0, 5]} color="white" />
        <OrbitControls target={center} />
        {/* XYZ Gizmo in corner */}
        <GizmoHelper alignment="top-right" margin={[80, 80]}>
          <GizmoViewport axisColors={['red', 'green', 'blue']} labelColor="white" />
        </GizmoHelper>
      </Canvas>
    </div>
    </div>
  </>
  )
}
