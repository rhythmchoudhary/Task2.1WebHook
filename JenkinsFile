pipeline {
    agent any

    environment {
        DIRECTORY_PATH = "https://github.com/rhythmchoudhary/Task-1.1P.git"
        TESTING_ENVIRONMENT = "Testing_Env"
        PRODUCTION_ENVIRONMENT = "Rhythm_choudhary"  // Change this to your name
        RECIPIENT_EMAIL = "rhythmxa3011@gmail.com"
        LOG_FILE = "jenkins-log.txt"
    }

    stages {
        stage('Checkout') {
            steps {
                script {
                    echo "Fetching source code from: ${env.DIRECTORY_PATH}"
                    checkout([$class: 'GitSCM', branches: [[name: '*/main']], 
                              userRemoteConfigs: [[url: env.DIRECTORY_PATH]]])
                }
            }
        }

        stage('Build') {
            steps {
                echo "Compiling the code and generating artifacts"
            }
        }

        stage('Unit and Integration Tests') {
            steps {
                echo "Running unit and integration tests"
            }
            post {
                always {
                    script {
                        def logs = currentBuild.rawBuild.getLog(100).join("\n")
                        writeFile file: "${LOG_FILE}", text: logs
                    }
                    archiveArtifacts artifacts: "${LOG_FILE}", fingerprint: true
                    emailext subject: "Jenkins Test Results",
                             body: "Unit & Integration tests completed. Log file attached.",
                             to: "${env.RECIPIENT_EMAIL}",
                             attachmentsPattern: "${LOG_FILE}",
                             mimeType: 'text/plain'
                }
            }
        }

        stage('Code Quality Check') {
            steps {
                echo "Analyzing code quality using SonarQube"
            }
        }

        stage('Security Scan') {
            steps {
                echo "Performing security scan using OWASP Dependency Check"
            }
            post {
                always {
                    script {
                        def logs = currentBuild.rawBuild.getLog(100).join("\n")
                        writeFile file: "${LOG_FILE}", text: logs
                    }
                    archiveArtifacts artifacts: "${LOG_FILE}", fingerprint: true
                    emailext subject: "Jenkins Security Scan Results",
                             body: "Security scan completed. Log file attached.",
                             to: "${env.RECIPIENT_EMAIL}",
                             attachmentsPattern: "${LOG_FILE}",
                             mimeType: 'text/plain'
                }
            }
        }

        stage('Deploy to Staging') {
            steps {
                echo "Deploying the application to staging: ${env.TESTING_ENVIRONMENT}"
            }
        }

        stage('Integration Tests on Staging') {
            steps {
                echo "Running integration tests on staging"
            }
        }

        stage('Deploy to Production') {
            steps {
                echo "Deploying application to production: ${env.PRODUCTION_ENVIRONMENT}"
            }
        }
    }

    post {
        success {
            script {
                def logs = currentBuild.rawBuild.getLog(100).join("\n")
                writeFile file: "${LOG_FILE}", text: logs
            }
            archiveArtifacts artifacts: "${LOG_FILE}", fingerprint: true
            emailext subject: "Jenkins Pipeline Success",
                     body: "Pipeline executed successfully! Log file attached.",
                     to: "${env.RECIPIENT_EMAIL}",
                     attachmentsPattern: "${LOG_FILE}",
                     mimeType: 'text/plain'
        }
        failure {
            script {
                def logs = currentBuild.rawBuild.getLog(100).join("\n")
                writeFile file: "${LOG_FILE}", text: logs
                echo logs
            }
            archiveArtifacts artifacts: "${LOG_FILE}", fingerprint: true
            emailext subject: "Jenkins Pipeline Failure",
                     body: "Pipeline execution failed. Log file attached.",
                     to: "${env.RECIPIENT_EMAIL}",
                     attachmentsPattern: "${LOG_FILE}",
                     mimeType: 'text/plain'
        }
    }
}
