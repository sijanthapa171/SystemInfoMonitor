# System Architecture Documentation

This document outlines the architecture and components of the SystemInfoMonitor application.

## High-Level Architecture

The following diagram shows the high-level architecture of the SystemInfoMonitor:

```mermaid
graph TD
    A["SystemInfoMonitor"]
    B["CPU Monitor"]
    C["Memory Monitor"]
    D["System Stats"]
    E["Data Collection"]
    F["Display Interface"]
    
    A --> B
    A --> C
    A --> D
    B --> E
    C --> E
    D --> E
    E --> F

    style A stroke:#333,stroke-width:2px
    style F stroke:#333,stroke-width:2px
```

## Component Flow

The following diagram shows the data flow between components:

```mermaid
sequenceDiagram
    participant UI as User Interface
    participant MC as Main Controller
    participant CPU as CPU Monitor
    participant MEM as Memory Monitor
    participant SYS as System Stats
    
    UI->>MC: Request System Info
    MC->>CPU: Get CPU Stats
    CPU-->>MC: CPU Usage Data
    MC->>MEM: Get Memory Stats
    MEM-->>MC: Memory Usage Data
    MC->>SYS: Get System Stats
    SYS-->>MC: System Information
    MC-->>UI: Display Combined Info
```

## Module Structure

Here's the module organization of our application:

```mermaid
graph LR
    A[Main Application] --> B[Core Modules]
    B --> C[CPU Monitor]
    B --> D[Memory Monitor]
    B --> E[System Stats]
    
    A --> F[Utils]
    F --> G[Data Collection]
    F --> H[Display Utils]
    
    A --> I[Interface]
    I --> J[CLI Interface]
    I --> K[Display Formatting]

    style A stroke:#333,stroke-width:2px
```

## Data Flow Architecture

This diagram shows how data flows through the system:

```mermaid
flowchart TD
    A[Hardware Info] --> B[System Calls]
    B --> C[Data Collection Layer]
    C --> D[Data Processing]
    D --> E[Data Storage]
    E --> F[Display Layer]
    F --> G[User Interface]
    
    style A stroke:#333,stroke-width:2px
    style G stroke:#333,stroke-width:2px
```

## Implementation Details

The SystemInfoMonitor is implemented in `C` and uses the following key components:

1. **Core Monitoring**
   - CPU usage tracking
   - Memory utilization
   - System statistics collection

2. **Data Management**
   - Real-time data collection
   - Data processing
   - Storage handling

3. **User Interface**
   - Command-line interface
   - Data visualization
   - User interaction handling 