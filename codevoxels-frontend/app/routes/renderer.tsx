import { Canvas } from '@react-three/fiber'
import { useEffect, useMemo, useRef, useState } from 'react';
import { OrbitControls, GizmoHelper, GizmoViewport } from '@react-three/drei';
import * as THREE from 'three';

import './renderer.css';
import './global.css';

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

      <ambientLight intensity={1.0} />
      <directionalLight position={[0, 0, 5]} color="white" />
      <OrbitControls target={center} />

      {/* XYZ Gizmo in corner */}
      <GizmoHelper alignment="top-right" margin={[80, 80]}>
        <GizmoViewport axisColors={['red', 'green', 'blue']} labelColor="white" />
      </GizmoHelper>
    </Canvas>
    </div>
  </>
  )
}
