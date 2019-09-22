pipeline {
    agent any

    stages {
        stage('Build') {
            steps {
                sh 'set -e
                    prepare.sh
                    cmake --build ./build'
            }
        }
        stage('Test') {
            steps {
                sh 'cmake --build ./build --target test'
            }
        }
    }
}
